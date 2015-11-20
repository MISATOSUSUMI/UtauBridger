/*
    wavtool-pl
    Copyright (C) 2011 Ying-Chun Liu (PaulLiu) <paulliu@debian.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../sndfile.h"

#include "wfh.h"
#include "wfd.h"

/**
 * Create an empty dat file
 *
 * @param filename the filename of dat file
 */
void wfd_init(const char *filename) {
  FILE *file1;
  file1 = fopen(filename,"w");
  if (file1) {
    fclose(file1);
  }
}

/**
 * convert micro seconds to samples
 *
 * @param time time in ms
 * @return samples
 */
int wfd_ms2samples(double time) {
  double samples_f;
  int samples;
  samples_f = ((double)wfh_samplerate) * time / 1000.0;
  samples = (int)samples_f;
  return samples;
}

/**
 * skip the file by microseconds
 *
 * @param inputfile the file descriptor
 * @param time time in ms
 */
void wfd_skip(FILE *inputfile, double time) {
  int samples;
  samples = wfd_ms2samples(time);
  fseek(inputfile,SEEK_CUR,samples*wfh_channels*(wfh_bits/8));
}

/**
 * Get volume factor by frame number
 *
 * @param frame Current frame number
 * @param p the p vector, 7 elements, sort by time (ms)
 * @param v the v vector, 7 elements, sort based on p (%)
 * @return the volume factor for frame. (%)
 */
double wfd_append_linear_volume(int frame, const int *p, const double *v) {
  double ret=0.0;
  int i;
  for (i=0; i<6; i++) {
    if (p[i] <= frame && frame < p[i+1]) {
      ret = v[i] + (v[i+1]-v[i]) * (((double)(frame-p[i])) / (((double)(p[i+1]-p[i]))));
      break;
    }
  }
  return ret;
}

/**
 * mix sample A with sample B
 *
 * @param a sample A
 * @param b sample B
 * @return sample of A mix B
 */
int16_t wfd_mix(int16_t a, int16_t b) {
  int a_t;
  int b_t;
  int r1;
  int16_t ret=0;

  a_t = ((int)a)+32768;
  b_t = ((int)b)+32768;

  if (a_t <= 32768 && b_t <= 32768) {
    r1 = (a_t * b_t) / 32768;
  } else {
    r1 = 2*(a_t+b_t) - (a_t*b_t)/32768 - 65536;
    if (r1>=65536) {
      r1 = 65535;
    }
  }
  r1 = r1 - 32768;
  ret = ((int16_t)r1);
  return ret;
}


/**
 * append data to dat file
 *
 * @param outputfilename the filename of output file
 * @param inputfilename the filename of input file
 * @param offset skip the ms in input file
 * @param length the time in ms have to be handled
 * @param ovr the overlap of output file and input file (ms)
 * @param p the p position array
 * @param v the v volume array
 * @return 0 - ok, nonzero - error
 */
int wfd_append(const char *outputfilename, const char *inputfilename,
	        double offset, double length,
		double ovr, const double *p, const double *v) {
  int ret=0;
  FILE *outfile=NULL;
  SNDFILE *inputfile=NULL;
  SF_INFO inputfileinfo;
  int16_t *buf=NULL;
  int p_f[7];
  double v_f[7];
  int currentFrame=0;
  int ovrFrames=0;
  int outputFrames=0;
  int i;
  int16_t sum;
  int c1,c2;

  memset(&inputfileinfo,0,sizeof(SF_INFO));
  outfile = fopen(outputfilename,"r+b");
  fseek(outfile,0,SEEK_END);
  if (inputfilename) {
    inputfile = sf_open(inputfilename,SFM_READ,&inputfileinfo);
    sf_command(inputfile,SFC_SET_SCALE_FLOAT_INT_READ, NULL, SF_TRUE);
  } else {
    inputfile=NULL;
  }
  outputFrames = wfd_ms2samples(length);

  /* handle offset */
  if (wfd_ms2samples(offset)>0) {
    if (inputfile) {
      sf_seek(inputfile,wfd_ms2samples(offset),SEEK_SET);
    }
  }

  /* pre-calculate volume */
  p_f[0]=0;
  for (i=0; i<2; i++) {
    p_f[i+1] = wfd_ms2samples(p[i]) + p_f[i];
  }
  p_f[3] = wfd_ms2samples(p[4]) + p_f[2];
  p_f[6] = outputFrames;
  p_f[5] = p_f[6] - wfd_ms2samples(p[3]);
  p_f[4] = p_f[5] - wfd_ms2samples(p[2]);

  v_f[0]=0.0;
  for (i=0;i<2; i++) {
    v_f[i+1] = v[i];
  }
  v_f[3]=v[4];
  v_f[4]=v[2];
  v_f[5]=v[3];
  v_f[6]=0.0;

  if (p_f[1]==p_f[2]) {
    v_f[1]=v_f[2];
  }
  if (p_f[0]==p_f[1]) {
    v_f[0]=v_f[1];
  }
  if (p_f[5]==p_f[4]) {
    v_f[5]=v_f[4];
  }
  if (p_f[6]==p_f[5]) {
    v_f[6]=v_f[5];
  }

  ovrFrames = wfd_ms2samples(ovr);
  if (ovrFrames > 0) {
    fseek(outfile,(-1)*wfh_channels*(wfh_bits/8)*ovrFrames,SEEK_END);
  } else if (ovr < 0.0) {
    /* output blank samples */
    int ovrSamples=0;
    int i,j,k;
    ovrSamples = wfd_ms2samples(-ovr);
    for (i=0; i<ovrSamples; i++) {
      for (j=0; j<wfh_channels; j++) {
	for (k=0; k<(wfh_bits/8); k++) {
	  fwrite("\0",1,1,outfile);
	}
      }
    }
    fflush(outfile);
    fseek(outfile,0,SEEK_CUR);
    ovr=0.0;
    ovrFrames=0;
  }

  /* output */
  buf = (int16_t *)malloc(sizeof(int16_t)*(inputfileinfo.channels));
  memset(buf,0,sizeof(int16_t)*(inputfileinfo.channels));

  currentFrame = 0;
  for ( ; outputFrames > 0; outputFrames--) {
    if (inputfile) {
      int result1;
      result1 = (int)sf_readf_short(inputfile,buf,1);
      if (result1 < 1) {
	memset(buf,0,sizeof(int16_t)*(inputfileinfo.channels));
	sf_close(inputfile);
	inputfile=NULL;
      }
    }
    /* simple mix if there are multi-channels */
    sum=0;
    for (i=0; i<inputfileinfo.channels; i++) {
      sum += buf[i];
    }
    /* modify the volume */
    if (inputfileinfo.channels>0) {
      double vf;
      sum = sum/inputfileinfo.channels;
      vf = wfd_append_linear_volume(currentFrame,p_f,v_f);
      sum = (int16_t)(((double)sum)*(vf/100.0));
    } else {
      sum=0;
    }
    if (ovrFrames>0) {
      int16_t d,r;
      c1 = fgetc(outfile);
      if (c1 == EOF) {
	ovrFrames=0;
	goto wfd_append_normal;
      }
      c2 = fgetc(outfile);
      if (c2 == EOF) {
	ovrFrames=0;
	goto wfd_append_normal;
      }
      fseek(outfile,-2,SEEK_CUR);
      d = (c1 & (0x00ff)) | (((c2 & 0x00ff) << 8) & 0xff00);
      r = wfd_mix(sum,d);
      fputc( (int)((r) & (0x00ff)), outfile);
      fputc( (int)((r>>8) & 0x00ff), outfile);
      fflush(outfile);
      fseek(outfile,0,SEEK_CUR);
      ovrFrames--;
    } else {
    wfd_append_normal:
      fputc( (int)((sum) & (0x00ff)), outfile);
      fputc( (int)((sum>>8) & 0x00ff), outfile);
      fflush(outfile);
      fseek(outfile,0,SEEK_CUR);
    }
    currentFrame++;
  }

  if (!inputfile) {
    sf_close(inputfile);
    inputfile=NULL;
  }
  if (!buf) {
    free(buf);
    buf=NULL;
  }
  ret = ftell(outfile);
  fclose(outfile);
  outfile=NULL;
  return ret;
}
