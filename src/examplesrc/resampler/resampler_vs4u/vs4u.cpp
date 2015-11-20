#include <Windows.h>
#include "resampler.h"
#include "vs4u_main.h"

//Resampler合成过程，参数与普通EXE参数顺序相同，最长14个参数，可约定
extern "C" int WINAPI Exec(char* inputWaveFile, char* outputWaveFile, char* NoteStr, char* Velocity, char* Flags, char* Offset, char* RequiredLength, char* FixedLength, char* EndBlank, char* Volume, char* Modulation, char* Tempo, char* PitchString, int aSign)
{
	char* argv[14]={"",inputWaveFile,outputWaveFile,NoteStr,Velocity,Flags,Offset,RequiredLength,FixedLength,EndBlank,Volume,Modulation,Tempo,PitchString};//不必理会第一个参数，第一个参数是EXE入口的EXE文件本身路径
	int ret=mainproc(14,argv);
	return ret;
}

//判断发音是否存在，第一个参数为默认发音，第二个是索引标记，恒为0。目前该参数与合成无直接关联，直接返回1即可。
extern "C" int WINAPI IsExists(char* DefaultLyric,int isSign)
{
	return 1;
}