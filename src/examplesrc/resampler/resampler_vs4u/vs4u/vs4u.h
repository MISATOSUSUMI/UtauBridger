
//--------------------------------------------------------------
// VS4U
//--------------------------------------------------------------
#include "vslib.h"

#ifndef _VS4U_H_
#define _VS4U_H_

//--------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------
#define M_PI			(3.14159265358979323846)

#define VS4U_MAX_PATH	(260)	// パスの長さ

#define VS4U_PBSIZE		(65536)	// ピッチベンドデータ用バッファサイズ(120BPMの場合341秒分)

//#define PRINT_TIME			// 処理時間表示
//#define PRINT_OPTION			// オプション表示


//--------------------------------------------------------------
// 構造体定義
//--------------------------------------------------------------

// オプションなど必要なデータをVS4UOPTIONに定義する
typedef struct{
	// ファイル名
	char inputWaveFile[VS4U_MAX_PATH];	// 入力WAVEファイル
	char outputWaveFile[VS4U_MAX_PATH];	// 出力WAVEファイル
	char frqFile[VS4U_MAX_PATH];		// 標準周波数表ファイル
	char vsfrqFile[VS4U_MAX_PATH];		// 独自周波数表ファイル
	char vspFile[VS4U_MAX_PATH];		// プロジェクトファイル(デバッグ用)

	// オプション
	int noteNum;				// 出力音階[ノートナンバー(C4=60)]
	double offset;				// オフセット[ms]
	double reqLength;			// 要求長[ms]
	double fixedLength;			// 子音部[ms]
	double endBlank;			// ブランク[ms](負の場合、オフセットからの相対値)
	double fixSpeed;			// 子音速度(0で2倍,100で1倍,200で0.5倍)

	double volume;				// ボリューム[%](0～200)
	double modulation;			// モジュレーション[%](-200～200)
	double tempo;				// テンポ[BPM](10～512)
	int nPitchBend;				// ピッチベンドデータ数	
	int pitchBend[VS4U_PBSIZE];	// ピッチベンドデータ[cent](-2048～2047)
								// ↑(高速化ツールを使って実行した際、環境によっては動的確保に
								//    失敗することがあるためスタックから確保。原因は不明)

	int noFormantFlg;			// フォルマント調整なしフラグ	("N",初期値OFF)
	int genderFactor;			// ジェンダーファクター[-20cent]("g",-100～100,初期値0)
	int pitchShift;				// ピッチ微調整[10cent]			("t",-240～240,初期値0)
	int pitchShift2;			// ピッチ微調整[1cent]			("T",-2400～2400,初期値0)
	int peekCmp;				// ピークコンプレッサ			("P",0～100,初期値86)
	int lowPassfilter;			// ローパスフィルタ				("H",0～99,初期値0)
	int breathiness;			// ブレシネス					("B",0～100,初期値50)
	int brightness;				// ブライトネス					("O",-100～100,初期値0)
	int clearness;				// クリアネス					("S",-100～100,初期値0)
	int outputVshpFlg;			// VSHP出力フラグ				("Z",初期値OFF)
	int reGenerateFrqFlg;		// 周波数表再作成フラグ			("V",初期値OFF)
	int disablePitchShift;		// ピッチシフト無効化フラグ		("W",基本値0)
								//	-2:和音モード,PS無効 -1:単音モード,PS無効 0:PS有効

	// 処理用
	short *tmpWaveData;			// 一時データ
	int tmpSample;				// 一時データサンプル数
	int tmpSmpFreq;				// 一時データのサンプリング周波数
} VS4UOPTION;

// 周波数データ
typedef struct{
	double freq;
	double dyn;
} FREQDATA;


//--------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------

// 共通
double LimitDoubleValue( double value, double min, double max );
int LimitIntValue( int value, int min, int max );
void AddExtStr( char *dstStr, int dstSize, char *extStr );
int CheckFileExist( char *fileName );

// オプション設定
int Vs4uGetOption( int argc, char *argv[], VS4UOPTION *option );
int Vs4uCnvNoteNum( char *nnStr );
int Vs4uSetFlgOption( char *flgStr, VS4UOPTION *option );
int Vs4uDecodeBase64( char *str );
int Vs4uSetPitchBendOption( char *argv[], VS4UOPTION *option );
void Vs4uPrintOption( VS4UOPTION *option );

// ピッチ、フォルマント、タイミング
int Vs4uStep1( VS4UOPTION *option );
HVSPRJ Vs4uLoadProject( VS4UOPTION *option );
int Vs4uSetTimeStretchPrm( HVSPRJ hVsprj, VS4UOPTION *option );
int Vs4uLoadFrqFile( HVSPRJ hVsprj, VS4UOPTION *option );
int Vs4uCalcAveragePit( HVSPRJ hVsprj, VS4UOPTION *option );
int Vs4uSetPitPrm( HVSPRJ hVsprj, VS4UOPTION *option );

// ローパスフィルタ、ボリューム
int Vs4uStep2( VS4UOPTION *option );
int Vs4uLowPassFilter( VS4UOPTION *option );
int Vs4uVolume( VS4UOPTION *option );
int Vs4uOutputWaveFile( VS4UOPTION *option );

#endif /* _VS4U_H_ */

