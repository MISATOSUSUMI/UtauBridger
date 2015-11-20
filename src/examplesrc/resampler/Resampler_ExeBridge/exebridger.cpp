
#include <Windows.h>
#include <io.h>
#include "resampler.h"

char* exefile;
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//以下为初始化
		char* file=(char*)malloc(256);
		exefile=(char*)malloc(256);
		int fsize=GetModuleFileName(hModule,file,256);
		file[fsize-1]='e';
		file[fsize-2]='x';
		file[fsize-3]='e';
		if(access(file,0)==0)
		{
			strcpy(exefile,file);
		}else
		{
			exefile="*";
		}
	}
	return TRUE;
}

//Resampler合成过程，参数与普通EXE参数顺序相同，最长14个参数，可约定
extern "C" int WINAPI Exec(char* inputWaveFile, char* outputWaveFile, char* NoteStr, char* Velocity, char* Flags, char* Offset, char* RequiredLength, char* FixedLength, char* EndBlank, char* Volume, char* Modulation, char* Tempo, char* PitchString, int aSign)
{
		char* pbuffer=(char*)malloc(sizeof(char)*_MAX_PATH*4);
		wsprintf(pbuffer,"\"%s\" \"%s\" %s %s \"%s\" %s %s %s %s %s %s %s %s",
			inputWaveFile,outputWaveFile,NoteStr,Velocity,Flags,Offset,RequiredLength,FixedLength,EndBlank,Volume,Modulation,Tempo,PitchString);
	    SHELLEXECUTEINFO ShExecInfo = {0};
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        ShExecInfo.hwnd = NULL;
        ShExecInfo.lpVerb = "open";
        ShExecInfo.lpFile = exefile;
        ShExecInfo.lpParameters = pbuffer;
        ShExecInfo.lpDirectory = NULL;
        ShExecInfo.nShow = SW_HIDE;
        ShExecInfo.hInstApp = NULL;
        ShellExecuteEx(&ShExecInfo);
        WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		return 0;
}
//判断发音是否存在，第一个参数为默认发音，第二个是索引标记，恒为0。目前该参数与合成无直接关联，直接返回1即可。
extern "C" int WINAPI IsExists(char* DefaultLyric,int isSign)
{
	return 1;
}