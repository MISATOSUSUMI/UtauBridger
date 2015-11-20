
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数，以下函数用来与UTAU系统连接
#pragma comment(linker, "/EXPORT:exec=_Exec@56")
#pragma comment(linker, "/EXPORT:isexists=_IsExists@8")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Resampler合成过程，参数与普通EXE参数顺序相同，最长14个参数，可约定
extern "C" int WINAPI Exec(char* inputWaveFile, char* outputWaveFile, char* NoteStr, char* Velocity, char* Flags, char* Offset, char* RequiredLength, char* FixedLength, char* EndBlank, char* Volume, char* Modulation, char* Tempo, char* PitchString, int aSign);
//判断发音是否存在，第一个参数为默认发音，第二个是索引标记，恒为0。目前该参数与合成无直接关联，直接返回1即可。
extern "C" int WINAPI IsExists(char* DefaultLyric,int isSign);
