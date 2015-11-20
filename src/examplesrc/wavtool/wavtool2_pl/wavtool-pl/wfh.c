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
#include <string.h>
#include <stdlib.h>
#include "wfh.h"

int wfh_samplerate=44100;
int wfh_channels=1;
int wfh_bits=16;

/**
 * read an integer from file
 *
 * @param file the input file descriptor
 * @param len the size of the integer. 2: 2 bytes, 4: 4bytes.
 * @return the integer read from file
 */
int wfh_readint(FILE *file1,int len) {
  int ret=0;
  int i;
  int c;
  for (i=0; i<len; i++) {
    c = fgetc(file1);
    if (c == EOF) {
      break;
    }
    ret = ret + (int)((c & 0x00ff)*(0x01<<(8*i)));
  }
  fseek(file1,0,SEEK_CUR);
  return ret;
}

/**
 * write an integer from file
 *
 * @param file the output file descriptor
 * @param data the integer need to be write to the file
 * @param len the size of the integer. 2: 2 bytes, 4: 4bytes.
 */
void wfh_writeint(FILE *file1,int data, int len) {
  int i;
  int d;
  d = data;
  for (i=0; i<len; i++) {
    int w;
    w = d%256;
    d = d/256;
    fputc(w,file1);
  }
  fflush(file1);
  fseek(file1,0,SEEK_CUR);
}

/**
 * Init a wfh file
 *
 * @param filename the filename of the wfh file
 */
void wfh_init(const char *filename) {
  FILE *file1=NULL;
  file1 = fopen(filename,"wb");
  if (!file1) {
    return;
  }

  fputs("RIFF",file1);

  wfh_writeint(file1,0,4);

  fputs("WAVE",file1);
  fputs("fmt ",file1);

  wfh_writeint(file1,16,4);
  wfh_writeint(file1,1,2);
  wfh_writeint(file1,wfh_channels,2);
  wfh_writeint(file1,wfh_samplerate,4);
  wfh_writeint(file1,wfh_samplerate*(wfh_bits/8*wfh_channels),4);
  wfh_writeint(file1,wfh_bits/8*wfh_channels,2);
  wfh_writeint(file1,wfh_bits,2);

  fputs("data",file1);
  wfh_writeint(file1,0,4);

  fclose(file1);
}

/**
 * Get length from a wav file header
 *
 * @param filename the filename of the file
 * @return the length of data
 */
int wfh_getlength(const char *filename) {
  FILE *file1=NULL;
  int ret=-1;
  char com1[100];
  int s;
  file1=fopen(filename,"rb");
  if (!file1) {
    return ret;
  }

  if (fread(com1,1,4,file1) != 4) {
    fclose(file1);
    return ret;
  }

  if (com1[0] != 'R' || com1[1]!='I' || com1[2]!='F' || com1[3]!='F') {
    fclose(file1);
    return ret;
  }

  if (fseek(file1,4,SEEK_CUR)!=0) {
    fclose(file1);
    return ret;
  }

  if (fread(com1,1,4,file1) != 4) {
    fclose(file1);
    return ret;
  }

  if (com1[0] != 'W' || com1[1]!='A' || com1[2]!='V' || com1[3]!='E') {
    fclose(file1);
    return ret;
  }

  if (fread(com1,1,4,file1) != 4) {
    fclose(file1);
    return ret;
  }

  if (com1[0] != 'f' || com1[1]!='m' || com1[2]!='t' || com1[3]!=' ') {
    fclose(file1);
    return ret;
  }

  s = wfh_readint(file1,4);
  if (fseek(file1,s,SEEK_CUR)!=0) {
    fclose(file1);
    return ret;
  }

  if (fread(com1,1,4,file1) != 4) {
    fclose(file1);
    return ret;
  }

  if (com1[0] != 'd' || com1[1]!='a' || com1[2]!='t' || com1[3]!='a') {
    fclose(file1);
    return ret;
  }

  ret = wfh_readint(file1,4);
  fclose(file1);
  return ret;
}

/**
 * Modify the length of a wav file in header
 *
 * @param filename the filename of the file
 * @param length the length need to be write to the wavheader
 * @return 0: success, non-zero: fail
 */
int wfh_putlength(const char *filename, int length) {
  FILE *file1=NULL;
  int ret=-1;
  char com1[100];
  int s;
  file1=fopen(filename,"r+b");
  if (!file1) {
    return ret;
  }

  if (fread(com1,1,4,file1) != 4) {
    fclose(file1);
    return ret;
  }

  if (com1[0] != 'R' || com1[1]!='I' || com1[2]!='F' || com1[3]!='F') {
    fclose(file1);
    return ret;
  }

  /* put file size - 8 here */
  if (fseek(file1,4,SEEK_SET)!=0) {
    fclose(file1);
    return ret;
  }
  wfh_writeint(file1, 4+4+4+ 4+4+2+2+4+4+2+2+4+4+ length -8 ,4);
  if (fseek(file1,8,SEEK_SET)!=0) {
    fclose(file1);
    return ret;
  }

  if (fread(com1,1,4,file1) != 4) {
    fclose(file1);
    return ret;
  }

  if (com1[0] != 'W' || com1[1]!='A' || com1[2]!='V' || com1[3]!='E') {
    fclose(file1);
    return ret;
  }

  if (fread(com1,1,4,file1) != 4) {
    fclose(file1);
    return ret;
  }

  if (com1[0] != 'f' || com1[1]!='m' || com1[2]!='t' || com1[3]!=' ') {
    fclose(file1);
    return ret;
  }

  s = wfh_readint(file1,4);
  if (fseek(file1,s,SEEK_CUR)!=0) {
    fclose(file1);
    return ret;
  }

  if (fread(com1,1,4,file1) != 4) {
    fclose(file1);
    return ret;
  }

  if (com1[0] != 'd' || com1[1]!='a' || com1[2]!='t' || com1[3]!='a') {
    fclose(file1);
    return ret;
  }

  if (fseek(file1,40,SEEK_SET)!=0) {
    fclose(file1);
    return ret;
  }
  wfh_writeint(file1,length,4);

  fclose(file1);
  ret=0;
  return ret;
}
