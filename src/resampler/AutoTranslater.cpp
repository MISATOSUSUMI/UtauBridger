#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include "MD5Code.h"

char* getTempDir(char* inputWaveFile);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 用来利用CodePage转换编码
char* CodePageTransform(int sourceCodePage,int destCodePage,char* szAnsi)
{
	int wcsLen = ::MultiByteToWideChar(sourceCodePage,NULL,szAnsi,strlen(szAnsi),NULL,0);
	wchar_t* wszString = (wchar_t*)malloc(sizeof(wchar_t)*wcsLen+2);
	::MultiByteToWideChar(sourceCodePage,NULL,szAnsi,strlen(szAnsi),wszString,wcsLen);

	int cszLen = ::WideCharToMultiByte(destCodePage,NULL,wszString,wcsLen,NULL,0,NULL,NULL);
	char* ret=(char*)malloc(sizeof(char)*cszLen+1);
	::WideCharToMultiByte(destCodePage,NULL,wszString,wcsLen,ret,cszLen,NULL,NULL);
	ret[cszLen]='\0';
	return ret;
}

//尝试逐层查找递归，将路径传输

wchar_t* MB2WC(char* szAnsi,UINT ACP)
{
	int wcsLen = ::MultiByteToWideChar(ACP,NULL,szAnsi,strlen(szAnsi),NULL,0);
	wchar_t* wszString = (wchar_t*)malloc(sizeof(wchar_t)*wcsLen+2);
	::MultiByteToWideChar(ACP,NULL,szAnsi,strlen(szAnsi),wszString,wcsLen);
	wszString[wcsLen]='\0';
	return wszString;
}
#define HIBYTE(w) ((unsigned char)((wchar_t)(w) >> 8))
#define LOBYTE(w) ((unsigned char) (w))
void TransSimpleCodePage(char* sourcePath,char* destPath,UINT ACP)
{
	char* tempDir=getTempDir(sourcePath);
	//WAV
	wchar_t* wsourcePath=MB2WC(sourcePath,ACP);
	wchar_t* filep=wcsrchr(wsourcePath,'\\')+1;
	int filestrlen=wcslen(filep);
	char* pw=(char*)malloc(sizeof(wchar_t)*filestrlen);
	char* pi=pw;
	for(int i=0;i<=wcslen(filep);i++)
	{
		int k=filep[i];
		if(k>=0x0 && k<=0xFF)
		{
			*pi=k;
			pi++;
		}else if(k>=0x0 && k<=0xFFFF)
		{
			*pi='x';
			pi++;
			char* vc=itoa(k,pi,16);
			pi=pi+strlen(pi);
		}
	}
	*pi='\0';
	int opathlen=sizeof(char)*(strlen(tempDir)+2+strlen(pw));
	char* opath=(char*)malloc(opathlen);
	memset(opath,0,opathlen);
	sprintf(opath,"%s\\%s",tempDir,pw);
	if(access(opath,0)!=0)
	{
		wchar_t* opathW=MB2WC(opath,ACP);
		if(!CopyFileW(wsourcePath, opathW, TRUE))
		{
			strcpy(destPath,sourcePath);
			return;
		}
	}
	strcpy(destPath,opath);
}
void TransJPNPath(char* sourcePath,char* destPath,UINT ACP,BOOL ISWAV)
{
	if(access(sourcePath,0)!=0 && strlen(sourcePath)>3)
	{
			if(ISWAV && _waccess(MB2WC(sourcePath,ACP),0)==0)
			{
				TransSimpleCodePage(sourcePath,destPath,ACP);
				return;
			}
			//文件不存在
			//获取原始wav文件所在目录
			char* localDir=(char*)malloc(sizeof(char)*strlen(sourcePath));
			strcpy(localDir,sourcePath);
			//获取文件名和上层目录脕E
			char* tfile=(char*)malloc(sizeof(char)*100);
			char* tPtr=strrchr(localDir, '\\');
			strcpy(tfile,(tPtr+1));
			*tPtr=0;
			
			//递归修复路径
			TransJPNPath(localDir,localDir,ACP,FALSE);

			//修复当前地址脕E
			char* opath=(char*)malloc(sizeof(char)*strlen(sourcePath)+100);
			
			if(ACP!=932)
			{
				char* ShiftJIS=CodePageTransform(ACP,932,tfile);
				sprintf(opath,"%s\\%s",localDir,ShiftJIS);
				if(access(opath,0)!=0)
				{
					if(ISWAV  && _waccess(MB2WC(opath,ACP),0)==0)
					{
						TransSimpleCodePage(opath,destPath,ACP);
						return;
					}
					else
					{
						char* ACPJS=CodePageTransform(932,ACP,tfile);
						sprintf(opath,"%s\\%s",localDir,ACPJS);
						if(access(opath,0)!=0)
						{
							if(ISWAV  && _waccess(MB2WC(opath,ACP),0)==0)
							{
								TransSimpleCodePage(opath,destPath,ACP);
								return;
							}
							else
							{
								free(tfile);
								strcpy(destPath,sourcePath);
								//JIS->地址
								return;//判决文件不存在
							}
						}
					}
				}
				strcpy(destPath,opath);
				free(tfile);
			}else
			{
				strcpy(destPath,sourcePath);
			}
	}else
	{
		strcpy(destPath,sourcePath);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* lastMD5Path="";
char* lastTempPath="";

char* getTempDir(char* inputWaveFile)
{
	char* saveFolder=(char*)malloc(MAX_PATH);
	strcpy(saveFolder,inputWaveFile);
	*strrchr(saveFolder, '\\')='\0';
	
	if(strcmp(saveFolder,lastMD5Path)!=0)
	{
		lastMD5Path=(char*)malloc(sizeof(char)*strlen(saveFolder)+1);
		strcpy(lastMD5Path,saveFolder);

		unsigned char decrypt[16];      
		MD5_CTX md5;  
		MD5Init(&md5);                
		MD5Update(&md5,(unsigned char*)saveFolder,strlen(saveFolder));  
		MD5Final(&md5,decrypt);      

		char* TempDirPath=(char*)malloc(MAX_PATH);
		GetTempPath(MAX_PATH,TempDirPath);
		lastTempPath=(char*)malloc(MAX_PATH);
		
		char* lastTempDir=(char*)malloc(MAX_PATH);
		sprintf(lastTempDir,"%sUTAUDB",TempDirPath);

		sprintf(lastTempPath,"%sUTAUDB\\%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",TempDirPath
			,decrypt[0],decrypt[1],decrypt[2],decrypt[3],decrypt[4],decrypt[5],decrypt[6],
			decrypt[7],decrypt[8],decrypt[9],decrypt[10],decrypt[11],decrypt[12],decrypt[13],
			decrypt[14],decrypt[15]);  
		if(access(lastTempDir,0)!=0)
		{
			CreateDirectoryA(lastTempDir,NULL);
		}
		if(access(lastTempPath,0)!=0)
		{
			CreateDirectoryA(lastTempPath,NULL);
		}
	}
	strcpy(saveFolder,lastTempPath);
	return saveFolder;
}

void TranslateWAVPath(char* inputWaveFile,int ACP)
{
	char* localDir=(char*)malloc(MAX_PATH);
	memset(localDir,0,MAX_PATH);
	
	TransJPNPath(inputWaveFile,localDir,ACP,TRUE);
	strncpy(inputWaveFile,localDir,strlen(localDir));
	inputWaveFile[strlen(localDir)]='\0';
}