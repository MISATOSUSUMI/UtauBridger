////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 头文件
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include "AutoTranslater.h"
#include "resampler.h"
#include <locale.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Resampler合成过程，参数与普通EXE参数顺序相同，最长14个参数，可约定
typedef int( __stdcall *DLL_FUNC_Exec)(char* inputWaveFile, char* outputWaveFile, char* NoteStr, char* Velocity, char* Flags, char* Offset, char* RequiredLength, char* FixedLength, char* EndBlank, char* Volume, char* Modulation, char* Tempo, char* PitchString, int aSign);
HINSTANCE hdll=NULL;
DLL_FUNC_Exec D_Exec;

int AutoTranslater=1;
wchar_t* inifile;
wchar_t* currentEngine;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 入口函数
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//以下为初始化
		wchar_t* file=(wchar_t*)malloc(256);
		currentEngine=(wchar_t*)malloc(256);
		int fsize=GetModuleFileNameW(hModule,file,256);
		file[fsize-1]='i';
		file[fsize-2]='n';
		file[fsize-3]='i';
		inifile=new wchar_t[sizeof(wchar_t) * (wcslen(file)+1)];
		wcscpy(inifile,file);
		AutoTranslater=GetPrivateProfileIntW(L"Engine2",L"AutoCodePage",1,inifile);
		fsize=GetPrivateProfileStringW(L"Engine2",L"Resampler",L"",file,256,inifile);
		wcscpy(currentEngine,file);
		wchar_t* filedir=(wchar_t*)malloc(256);
		wcscpy(filedir,file);
		wchar_t* pfildir=wcsrchr(filedir,'\\');
		if(pfildir!=NULL)
		{
			*pfildir='\0';
			SetDllDirectoryW(filedir);
		}
		hdll=LoadLibraryW(file);
		D_Exec=(DLL_FUNC_Exec)GetProcAddress(hdll,"exec");
	} else if(dwReason == DLL_PROCESS_DETACH){		
		if(hdll!=NULL)FreeLibrary(hdll);
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void UpdateEngineTable()
{
	wchar_t* file=(wchar_t*)malloc(256);
	int str=GetPrivateProfileStringW(L"Engine2",L"Resampler",L"",file,256,inifile);	
	int diff=wcsicmp(file,currentEngine);
	if(diff!=0)
	{
		D_Exec=0;
		if(hdll!=NULL)FreeLibrary(hdll);
		wcscpy(currentEngine,file);
		hdll=LoadLibraryW(file);
		D_Exec=(DLL_FUNC_Exec)GetProcAddress(hdll,"exec");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数

extern "C" int WINAPI Exec(char* inputWaveFile, char* outputWaveFile, char* NoteStr, char* Velocity, char* Flags, char* Offset, char* RequiredLength, char* FixedLength, char* EndBlank, char* Volume, char* Modulation, char* Tempo, char* PitchString, int aSign)
{
	//说明，Exec函数参数池从左到右依次为Resampler的参数（觼Exe版本相同）。返回非0（原版为-1）时柄婢合成成功，返回0为失败。
	//以下为向其他resampler引擎搭桥
	int r=0;

	UpdateEngineTable();
	if(hdll != NULL && ((int)D_Exec)!=0)
	{
		//初始化地址池，尝试进行地址转换
		char* newInputPath=(char*)malloc(sizeof(char)*MAX_PATH);
		strcpy(newInputPath,inputWaveFile);
		if(AutoTranslater!=0 && access(newInputPath,0)!=0)
		{			
			int IniACP=GetPrivateProfileIntW(L"Engine2",L"SystemLangID",-1,inifile);
			if(IniACP==-1 || !IsValidCodePage(IniACP))
			{
				IniACP=GetACP();
				if(IniACP == 932)
				{
					char* ww=setlocale(LC_ALL,"");
					char* cp=strrchr(ww, '.')+1;
					int cpi=atoi(cp);
					if(IsValidCodePage(cpi))IniACP=cpi;
				}
			}
			TranslateWAVPath(newInputPath,IniACP);
		}
		r=D_Exec(newInputPath,outputWaveFile,NoteStr,Velocity,Flags,Offset,RequiredLength,FixedLength,EndBlank,Volume,Modulation,Tempo,PitchString,aSign);
		return r;
	}else
	{
		return 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数

extern "C" int WINAPI IsExists(char* DefaultLyric,int isSign)
{
	//说明：DefaultLyric一般为日文a，isSign为0，用来检查utau音源是否被设置。直接返回1
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

