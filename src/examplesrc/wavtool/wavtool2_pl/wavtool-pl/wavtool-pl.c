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
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../wavtool2.h"

#include "wfh.h"
#include "wfd.h"

struct wavtool_args_str {
  double offset;
  double length;
  double p[5];
  double v[5];
  double ovr;
  char *inputfilename;
  char *outputfilename;
} wavtool_args;

void printusage() {
  printf ("wavtool-pl <outfile> <infile> offset length"
          " p1 p2 p3 v1 v2 v3 v4 ovr p4 p5 v5\n");
}

int isFileExist(const char *filename) {
  int ret=0;
  FILE *file1=NULL;
  file1 = fopen(filename,"r");
  if (file1) {
    ret=1;
    fclose(file1);
  }
  return ret;
}

double parseLength(const char * lenstr) {
  int indexofAt=-1;
  int indexofPlus=-1;
  int i,len;
  double duration,tempo,plustime;
  char tmp1[1024];
  double ret=0.0;

  len = strlen(lenstr);

  for (i=0; i<len; i++) {
    if (lenstr[i]=='@') {
      indexofAt=i;
      break;
    }
  }

  for (i=len-1; i>=0; i--) {
    if (lenstr[i]=='+' || lenstr[i]=='-') {
      indexofPlus=i;
      break;
    }
  }

  if (indexofAt != -1 && indexofPlus != -1 && (indexofAt >= indexofPlus)) {
    return -1.0;
  }

  if (indexofAt == -1) {
    sscanf(lenstr,"%lf",&ret);
    if (ret<0.0) {
      ret = 0.0;
    }
    return ret;
  }

  if (indexofAt >= 1) {
    memset(tmp1,0,sizeof(tmp1));
    strncpy(tmp1,lenstr,indexofAt);
    sscanf(tmp1,"%lf",&duration);
  }

  if (indexofPlus == -1) {
    sscanf(&(lenstr[indexofAt+1]),"%lf",&tempo);
  } else {
    memset(tmp1,0,sizeof(tmp1));
    strncpy(tmp1,&(lenstr[indexofAt+1]),indexofPlus-indexofAt-1);
    sscanf(tmp1,"%lf",&tempo);
  }

  if (indexofPlus != -1) {
    strncpy(tmp1,&(lenstr[indexofPlus+1]),sizeof(tmp1)-1);
    sscanf(tmp1,"%lf",&plustime);
    if (lenstr[indexofPlus]=='-') {
      plustime = -plustime;
    }
  }

  if (tempo != 0.0) {
    ret = (1000.0 * (60.0 / tempo)) * duration / 480.0 + plustime;
  } else {
    ret = plustime;
  }
  if (ret < 0.0) {
    ret = 0.0;
  }
  return ret;
}

int parseArgs(int argc,char *argv[]) {
  int len;
  int i;
  static const int pindex[5]={5,6,7,13,14};
  static const int vindex[5]={8,9,10,11,15};

  memset(&wavtool_args,0,sizeof(wavtool_args));

  if (argc <= 2) {
    return -1;
  }
  
  len = strlen(argv[1]);
  wavtool_args.outputfilename = (char *)malloc(sizeof(char)*(len+1));
  memset(wavtool_args.outputfilename,0,sizeof(char)*(len+1));
  strncpy(wavtool_args.outputfilename, argv[1], len);
  
  if (isFileExist(argv[2])) {
    len = strlen(argv[2]);
    wavtool_args.inputfilename = (char *)malloc(sizeof(char)*(len+1));
    memset(wavtool_args.inputfilename,0,sizeof(char)*(len+1));
    strncpy(wavtool_args.inputfilename, argv[2], len);
  }

  if (argc >= 4) {
    sscanf(argv[3],"%lf",&(wavtool_args.offset));
  }

  if (argc >= 5) {
    wavtool_args.length = parseLength(argv[4]);
  }

  if (argc >= 13) {
    sscanf(argv[12],"%lf",&(wavtool_args.ovr));
  }

  for (i=0; i<5; i++) {
    if (argc >= pindex[i]+1) {
      sscanf(argv[pindex[i]],"%lf",&(wavtool_args.p[i]));
    }
    if (argc >= vindex[i]+1) {
      sscanf(argv[vindex[i]],"%lf",&(wavtool_args.v[i]));
    }
  }
  return 0;
}

void printArgs() {
  static const int printindex[5]={0,1,4,2,3};
  int i;
  printf ("Output: %s\n",wavtool_args.outputfilename);
  printf ("Input: %s\n",wavtool_args.inputfilename);
  printf ("Offset: %lf\n",wavtool_args.offset);
  printf ("Length: %lf, Ovr: %lf\n",wavtool_args.length,wavtool_args.ovr);
  for (i=0; i<5; i++) {
    printf ("p%d=%lf v%d=%lf%%\n",printindex[i]+1,
	    wavtool_args.p[printindex[i]],
	    printindex[i]+1,
	    wavtool_args.v[printindex[i]]);
  }
}

int mainprogram (int argc, char *argv[]) {

  char *outwavhdrfilename,*outwavdatfilename;
  int len;
  int result=0;

  if (parseArgs(argc,argv)!=0) {
    printusage();
    return 0;
  }

  len = strlen(wavtool_args.outputfilename)+4+1;
  outwavhdrfilename = (char *)malloc(len*sizeof(char));
  memset(outwavhdrfilename,0,len*sizeof(char));
  outwavdatfilename = (char *)malloc(len*sizeof(char));
  memset(outwavdatfilename,0,len*sizeof(char));
  _snprintf(outwavhdrfilename,len,"%s.whd",wavtool_args.outputfilename);
  _snprintf(outwavdatfilename,len,"%s.dat",wavtool_args.outputfilename);

  if (!isFileExist(outwavhdrfilename)) {
    wfh_init(outwavhdrfilename);
  }
  if (!isFileExist(outwavdatfilename)) {
    wfd_init(outwavdatfilename);
  }

  len = wfd_append(outwavdatfilename,wavtool_args.inputfilename,wavtool_args.offset,wavtool_args.length,wavtool_args.ovr,wavtool_args.p,wavtool_args.v);
  result = wfh_putlength(outwavhdrfilename,len);

  return 0;
}

//WAVTOOL过程执行函数，根据wavtool传参
int WINAPI Exec(char* outputWaveFile, char* beAppendWaveFile, char* StartPoint, char* LengthAtTempo, char* Env_P1, char* Env_P2, char* Env_P3, char* Env_V1, char* Env_V2, char* Env_V3, char* Env_V4, char* OverLap, char* Env_P4, char* Env_P5, char* Env_V5)
{
	char* argv[16]={"",outputWaveFile,beAppendWaveFile,StartPoint,LengthAtTempo,Env_P1,Env_P2,Env_P3,Env_V1,Env_V2,Env_V3,Env_V4,OverLap,Env_P4,Env_P5,Env_V5};//不必理会第一个参数，第一个参数是EXE入口的EXE文件本身路径
	int ret=mainprogram(16,argv);
	return ret;
}
