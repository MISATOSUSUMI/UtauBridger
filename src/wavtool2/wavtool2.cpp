////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 头文件
#include <Windows.h>
#include <stdio.h>
#include "wavtool2.h"
#include "appender.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef int( __stdcall *DLL_FUNC_Exec)(char* outputWaveFile, char* beAppendWaveFile, char* StartPoint, char* LengthAtTempo, char* Env_P1, char* Env_P2, char* Env_P3, char* Env_V1, char* Env_V2, char* Env_V3, char* Env_V4, char* OverLap, char* Env_P4, char* Env_P5, char* Env_V5);
//typedef int( __stdcall *DLL_FUNC_AppendWhdDat)(char* p1, char* p2, char* p3);

HINSTANCE hdll=NULL;
DLL_FUNC_Exec D_Exec;
//DLL_FUNC_AppendWhdDat D_Append;

wchar_t* inifile;
wchar_t* currentEngine;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 入口函数
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		wchar_t* file=(wchar_t*)malloc(256);
		currentEngine=(wchar_t*)malloc(256);
		DWORD fsize=GetModuleFileNameW(hModule,file,256);
		//resampler.dll
		//wavtool2.dll
		wchar_t* wfile=L"resampler.ini";
		memcpy((void*)((int)file+sizeof(wchar_t)*(fsize-12)),wfile,sizeof(wchar_t)*14);
		inifile=new wchar_t[wcslen(file)+1];
		wcscpy(inifile,file);
		fsize=GetPrivateProfileStringW(L"Engine2",L"Wavtool2",L"",file,256,inifile);
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
		//D_Append=(DLL_FUNC_AppendWhdDat)GetProcAddress(hdll,"append_files");
	} else if(dwReason == DLL_PROCESS_DETACH){		
		FreeLibrary(hdll);
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UpdateEngineTable()
{
	wchar_t* file=(wchar_t*)malloc(256);
	int str=GetPrivateProfileStringW(L"Engine2",L"Wavtool2",L"",file,256,inifile);
	int diff=wcsicmp(file,currentEngine);
	if(diff!=0)
	{
		D_Exec=0;
		//D_Append=0;
		if(hdll!=NULL)FreeLibrary(hdll);
		wcscpy(currentEngine,file);
		hdll=LoadLibraryW(file);
		D_Exec=(DLL_FUNC_Exec)GetProcAddress(hdll,"exec");
		//D_Append=(DLL_FUNC_AppendWhdDat)GetProcAddress(hdll,"append_files");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数

extern "C" int WINAPI Exec(char* outputWaveFile, char* beAppendWaveFile, char* StartPoint, char* LengthAtTempo, char* Env_P1, char* Env_P2, char* Env_P3, char* Env_V1, char* Env_V2, char* Env_V3, char* Env_V4, char* OverLap, char* Env_P4, char* Env_P5, char* Env_V5)
{
	//说明，Exec函数参数池从左到右依次为Wavtool的参数（与exe版本相同）。返回非0（原版为-1）时表示合成成功，返回0为失败。
	//以下为向其他Wavtool系统搭桥代码
	int r=0;

	UpdateEngineTable();
	if(hdll != NULL && ((int)D_Exec)!=0)
	{
		r=D_Exec(outputWaveFile,beAppendWaveFile,StartPoint,LengthAtTempo,Env_P1,Env_P2,Env_P3,Env_V1,Env_V2,Env_V3,Env_V4,OverLap,Env_P4,Env_P5,Env_V5);
		return r;
	}else
	{
		return 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
extern "C" int ModuleCheck()
{
	//说明，与resampler里对应项目相同，该函数用来验证加载状态的，返回0即可
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
extern "C" int WINAPI AppendWhdDat(char* whdFile, char* datFile, char* outputWave)
{
	//将wav文件头whd文件和连接的wav文件体dat连接，输出为outpuWave。调用此过程表示所有exec参数已经推送和执行完毕。该过程执行完成合并后会自动删除whd和dat文件
	//这个过程本来就是UTAU使用UTAU引擎的WAVTOOL2文件进行的，并不包含在wavtool.exe、wavtool4vcv、wavtoolex等子程序中。所以为了防止使用传统程序时渲染出错，BRIDGE程序将这部分代码内置了。
	//若有其他第三方开发者希望取代这部分程序，也应考虑这些。
	int ret;
	ret=WhdDatAppender(whdFile,datFile,outputWave);
    return ret;
}
/////////////