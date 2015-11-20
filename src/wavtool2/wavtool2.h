////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
#pragma comment(linker, "/EXPORT:moduleloader=_ModuleCheck")
#pragma comment(linker, "/EXPORT:append_files=_AppendWhdDat@12")
#pragma comment(linker, "/EXPORT:exec=_Exec@60")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//WAVTOOL过程执行函数，根据wavtool传参
extern "C" int WINAPI Exec(char* outputWaveFile, char* beAppendWaveFile, char* StartPoint, char* LengthAtTempo, char* Env_P1, char* Env_P2, char* Env_P3, char* Env_V1, char* Env_V2, char* Env_V3, char* Env_V4, char* OverLap, char* Env_P4, char* Env_P5, char* Env_V5);
//WAV文件合并过程结束函数，合并wavtool过程生成的wav头（temp.whd)和被连接的wav文件体（temp.dat)到目标文件(temp.wav)。标志着wavtool合成过程最终结束
extern "C" int WINAPI AppendWhdDat(char* whdFile, char* datFile, char* outputWave);
//模块检测函数，检测模块是否正常和存在，永久返回0;
extern "C" int ModuleCheck();

