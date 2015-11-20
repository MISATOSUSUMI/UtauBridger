#include <Windows.h>
#include <io.h>
#include "wavtool2.h"

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

extern "C" int WINAPI Exec(char* outputWaveFile, char* beAppendWaveFile, char* StartPoint, char* LengthAtTempo, char* Env_P1, char* Env_P2, char* Env_P3, char* Env_V1, char* Env_V2, char* Env_V3, char* Env_V4, char* OverLap, char* Env_P4, char* Env_P5, char* Env_V5)
{
		char* pbuffer=(char*)malloc(sizeof(char)*_MAX_PATH*4);
		wsprintf(pbuffer,"\"%s\" \"%s\" %s %s %s %s %s %s %s %s %s %s %s %s %s",
			outputWaveFile,beAppendWaveFile,StartPoint,LengthAtTempo,Env_P1,Env_P2,Env_P3,Env_V1,Env_V2,Env_V3,Env_V4,OverLap,Env_P4,Env_P5,Env_V5);
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