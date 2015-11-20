#include <Windows.h>
#include <stdio.h>

#define BufferSize 8192

size_t filewriter(const void *Str, size_t _Size, size_t _Count, FILE *fp)
{
  size_t ret; 
  _lock_file(fp);
  ret = fwrite(Str, _Size, _Count, fp);
  _unlock_file(fp);
  return ret;
}
size_t filereader(void *dstBuf, size_t _Size, size_t _Count, FILE *fp)
{
  size_t ret; 
  _lock_file(fp);
  ret = fread(dstBuf, _Size, _Count, fp);
  _unlock_file(fp);
  return ret;
}

int WhdDatAppender(char* whdFile, char* datFile, char* outputWave)
{
	FILE* outfile;
	FILE* whdfile;
	FILE* datfile;

	outfile=fopen((const char*)outputWave,"wb");
	if(outfile==NULL)return -1;
	whdfile=fopen((const char*)whdFile,"rb");
	if(whdfile==NULL){fclose(outfile);return -2;}
	datfile=fopen((const char*)datFile,"rb");
	if(datfile==NULL){fclose(whdfile);fclose(outfile);return -2;}
	
	BYTE* datBuffer=(BYTE*)malloc(sizeof(BYTE)*BufferSize);
	while(!feof(whdfile))
	{
		int datC=filereader(datBuffer,1,BufferSize,whdfile);
		if(datC>0)
		{
			filewriter(datBuffer,1,datC,outfile);
		}
	}
	fclose(whdfile);
	
	while(!feof(datfile))
	{
		int datC=filereader(datBuffer,1,BufferSize,datfile);
		if(datC>0)
		{
			filewriter(datBuffer,1,datC,outfile);
		}
	}
	fclose(datfile);
	fclose(outfile);
}