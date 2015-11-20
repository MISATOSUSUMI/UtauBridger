
//--------------------------------------------------------------
// VocalShifter - VSLIB
//--------------------------------------------------------------

#ifndef _VSLIB_H_
#define _VSLIB_H_

#pragma comment( lib, "lib\\vslib.lib" )

#ifdef __cplusplus
#define VSLIB_IMPORT extern "C" __declspec(dllimport)
#else
#define VSLIB_IMPORT __declspec(dllimport)
#endif

//--------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------

#define VSLIB_MAX_PATH		(256)
#define VSLIB_MAX_TRACK		(64)	// トラック最大数
#define VSLIB_MAX_ITEM		(1024)	// アイテム最大数

// エラーコード
#define VSERR_NOERR			(0)		// エラーなし
#define VSERR_PRM			(1)		// パラメータ不正
#define VSERR_PRJOPEN		(2)		// PRJファイルオープンに失敗
#define VSERR_PRJFORMAT		(3)		// PRJファイルフォーマット不正
#define VSERR_WAVEOPEN		(4)		// WAVEファイルオープンに失敗
#define VSERR_WAVEFORMAT	(5)		// WAVEファイルフォーマット不正
#define VSERR_FREQ			(6)		// サンプリング周波数不正
#define VSERR_MAX			(7)		// 最大数制限
#define VSERR_NOMEM			(8)		// メモリ不足

// 音声合成方式
#define SYNTHMODE_M			(0)		// 単音
#define SYNTHMODE_MF		(1)		// 単音フォルマント補正
#define SYNTHMODE_P			(2)		// 和音

// ハンドル定義
typedef struct HVSPRJ__ { int unused; } *HVSPRJ;

// ASAnalyzer チャンネル
#define AS_CH_LPR			(0)
#define AS_CH_LMR			(1)
#define AS_CH_L				(2)
#define AS_CH_R				(3)

// ASAnalyzer エラーコード
#define AS_ERR_NOERR		(0)
#define AS_ERR_NOMEM		(1)
#define AS_ERR_BADPRM		(2)
#define AS_ERR_OPEN			(3)
#define AS_ERR_FROMAT		(4)


//--------------------------------------------------------------
// 構造体定義
//--------------------------------------------------------------

// プロジェクト情報
typedef struct{
	double masterVolume;	// (R/W) マスターボリューム[倍]
	int sampFreq;			// (R/W) サンプリング周波数[Hz]
} VSPRJINFO;

// トラック情報
typedef struct{
	double volume;			// (R/W) ボリューム[倍]
	double pan;				// (R/W) パン(-1.0～1.0)
	int invPhaseFlg;		// (R/W) 逆相フラグ
	int soloFlg;			// (R/W) ソロフラグ
	int muteFlg;			// (R/W) ミュートフラグ
} VSTRACKINFO;

// アイテム情報
typedef struct{
	char fileName[VSLIB_MAX_PATH];	// (R/-) ファイル名
	int sampFreq;			// (R/-) サンプリング周波数[Hz]
	int channel;			// (R/-) チャンネル数
	int sampleOrg;			// (R/-) オリジナルの音声ファイルのサンプル数
	int sampleEdit;			// (R/-) 編集後のサンプル数
	int ctrlPntPs;			// (R/-) 1秒あたりの制御点数
	int ctrlPntNum;			// (R/-) 全制御点数
	int synthMode;			// (R/W) 音声合成方式
	int trackNum;			// (R/W) トラック番号
	int offset;				// (R/W) オフセット[sample]
} VSITEMINFO;

// アイテム制御点情報
typedef struct{
	double dynOrg;			// (R/-) 編集前ダイナミクス[倍]
	double dynEdit;			// (R/W) 編集後ダイナミクス[倍]
	double volume;			// (R/W) ボリューム[倍]
	double pan;				// (R/W) パン(-1.0～1.0)
	double spcDyn;			// (R/-) スペクトルダイナミクス
	int pitAna;				// (R/-) ピッチ解析値[cent]
	int pitOrg;				// (R/W) 編集前ピッチ[cent]
	int pitEdit;			// (R/W) 編集後ピッチ[cent]
	int formant;			// (R/W) フォルマント[cent]
	int pitFlgOrg;			// (R/-) 編集前ピッチ有無フラグ
	int pitFlgEdit;			// (R/W) 編集後ピッチ有無フラグ
	int breathiness;		// (R/W) ブレシネス[-10000～10000]
	int eq1;				// (R/W) EQ1[-10000～10000]
	int eq2;				// (R/W) EQ2[-10000～10000]
} VSCPINFOEX;

// ASAnalyzer AWDINFO構造体
typedef struct{
	int wavdatasize;	// データのサイズ[サンプル]
	int wavsampleps;	// サンプリング周波数[Hz]
	int wavbit;			// ビット数(8or16)
	int wavchannel;		// チャンネル数(1or2)
	int nnoffset;		// 解析する最低音[ノートナンバー]
	int nnrange;		// 解析する音階の範囲[半音]
	int blockpn;		// 半音あたりのブロック数(1-20)
	int dynsize;		// 音量検出区間[サンプル]
	int targetch;		// 対象のチャンネル
} AWDINFO;


//--------------------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------------------

// ライブラリ
VSLIB_IMPORT int __stdcall VslibGetVersion( void );

// プロジェクト
VSLIB_IMPORT int __stdcall VslibCreateProject( HVSPRJ *hVsprj );
VSLIB_IMPORT int __stdcall VslibOpenProject( HVSPRJ *hVsprj, char *fileName );
VSLIB_IMPORT int __stdcall VslibSaveProject( HVSPRJ hVsprj, char *fileName );
VSLIB_IMPORT int __stdcall VslibDeleteProject( HVSPRJ hVsprj );
VSLIB_IMPORT int __stdcall VslibGetProjectInfo( HVSPRJ hVsprj, VSPRJINFO *vsPrjInfo );
VSLIB_IMPORT int __stdcall VslibSetProjectInfo( HVSPRJ hVsprj, VSPRJINFO *vsPrjInfo );

// トラック
VSLIB_IMPORT int __stdcall VslibGetTrackMaxNum( HVSPRJ hVsprj, int *trackMaxNum );
VSLIB_IMPORT int __stdcall VslibAddTrack( HVSPRJ hVsprj );
VSLIB_IMPORT int __stdcall VslibCopyTrack( HVSPRJ hVsprjDst, int *trackNumDst, HVSPRJ hVsprjSrc, int trackNumsrc );
VSLIB_IMPORT int __stdcall VslibUpTrack( HVSPRJ hVsprj, int trackNum );
VSLIB_IMPORT int __stdcall VslibDownTrack( HVSPRJ hVsprj, int trackNum );
VSLIB_IMPORT int __stdcall VslibDeleteTrack( HVSPRJ hVsprj, int trackNum );
VSLIB_IMPORT int __stdcall VslibGetTrackInfo( HVSPRJ hVsprj, int trackNum, VSTRACKINFO *vsTrackInfo );
VSLIB_IMPORT int __stdcall VslibSetTrackInfo( HVSPRJ hVsprj, int trackNum, VSTRACKINFO *vsTrackInfo );

// アイテム
VSLIB_IMPORT int __stdcall VslibGetItemMaxNum( HVSPRJ hVsprj, int *itemMaxNum );
VSLIB_IMPORT int __stdcall VslibAddItem( HVSPRJ hVsprj, char *fileName, int *itemNum );
VSLIB_IMPORT int __stdcall VslibAddItemEx( HVSPRJ hVsprj, char *fileName, int *itemNum,
									int nnOffset, int nnRange, int skipPitFlg );
VSLIB_IMPORT int __stdcall VslibCopyItem( HVSPRJ hVsprjDst, int *itemNumDst, HVSPRJ hVsprjSrc, int itemNumSrc );
VSLIB_IMPORT int __stdcall VslibDeleteItem( HVSPRJ hVsprj, int itemNum );
VSLIB_IMPORT int __stdcall VslibGetItemInfo( HVSPRJ hVsprj, int itemNum, VSITEMINFO *vsItemInfo );
VSLIB_IMPORT int __stdcall VslibSetItemInfo( HVSPRJ hVsprj, int itemNum, VSITEMINFO *vsItemInfo );
VSLIB_IMPORT int __stdcall VslibGetCtrlPntInfoEx( HVSPRJ hVsprj, int itemNum, int pntNum, VSCPINFOEX *vsCpInfo );
VSLIB_IMPORT int __stdcall VslibSetCtrlPntInfoEx( HVSPRJ hVsprj, int itemNum, int pntNum, VSCPINFOEX *vsCpInfo );

VSLIB_IMPORT int __stdcall VslibGetEQGain( HVSPRJ hVsprj, int itemNum, int eqNum, int *gain );
VSLIB_IMPORT int __stdcall VslibSetEQGain( HVSPRJ hVsprj, int itemNum, int eqNum, int *gain );

// タイミング
VSLIB_IMPORT int __stdcall VslibGetTimeCtrlPntNum( HVSPRJ hVsprj, int itemNum, int *timeCtrlPntNum );
VSLIB_IMPORT int __stdcall VslibGetTimeCtrlPnt( HVSPRJ hVsprj, int itemNum, int pntNum, int *time1, int *time2 );
VSLIB_IMPORT int __stdcall VslibSetTimeCtrlPnt( HVSPRJ hVsprj, int itemNum, int pntNum, int time1, int time2 );
VSLIB_IMPORT int __stdcall VslibAddTimeCtrlPnt( HVSPRJ hVsprj, int itemNum, int time1, int time2 );
VSLIB_IMPORT int __stdcall VslibDeleteTimeCtrlPnt( HVSPRJ hVsprj, int itemNum, int pntNum );

VSLIB_IMPORT int __stdcall VslibGetStretchOrgSec( HVSPRJ hVsprj, int itemNum, double time_edt, double *time_org );
VSLIB_IMPORT int __stdcall VslibGetStretchEditSec( HVSPRJ hVsprj, int itemNum, double time_org, double *time_edt );
VSLIB_IMPORT int __stdcall VslibGetStretchOrgSample( HVSPRJ hVsprj, int itemNum, double time_edt, double *time_org );
VSLIB_IMPORT int __stdcall VslibGetStretchEditSample( HVSPRJ hVsprj, int itemNum, double time_org, double *time_edt );

// ミキサー
VSLIB_IMPORT int __stdcall VslibGetMixSample( HVSPRJ hVsprj, int *mixSample );
VSLIB_IMPORT int __stdcall VslibGetMixData( HVSPRJ hVsprj, void *waveData, int bit, int channel, int index, int size );
VSLIB_IMPORT int __stdcall VslibExportWaveFile( HVSPRJ hVsprj, char *fileName, int bit, int channel );

// 単位変換
VSLIB_IMPORT double __stdcall VslibCent2Freq( int cent );
VSLIB_IMPORT int __stdcall VslibFreq2Cent( double freq );
VSLIB_IMPORT double __stdcall VslibNoteNum2Freq( int noteNum );
VSLIB_IMPORT int __stdcall VslibFreq2NoteNum( double freq );

// その他
VSLIB_IMPORT int __stdcall VslibSetPitchArray( HVSPRJ hVsprj, int itemNum, int *pitData, int nPitData, double interval );

// ASAnalyzer
VSLIB_IMPORT int __stdcall AS_GetDllVer( void );
VSLIB_IMPORT double __stdcall AS_Cent2Freq( int cent, double freqa4 );
VSLIB_IMPORT int __stdcall AS_Freq2Cent( double freq, double freqa4 );
VSLIB_IMPORT double __stdcall AS_Nnum2Freq( int nnum, double freqa4 );
VSLIB_IMPORT int __stdcall AS_Freq2Nnum( double freq, double freqa4 );

VSLIB_IMPORT int __stdcall AS_AnalyzeWaveData( void *wavdata, unsigned short sndspc[], int *pitch, int wavdatasize,
	int wavsampleps, int wavbit, int wavchannel, int nnoffset, int nnrange, int blockpn, int targetch, double freqa4 );
VSLIB_IMPORT int __stdcall AS_AnalyzeWaveDataEX( void *wavdata, unsigned short *sndspc,
	int *pitch, int *dynamics, AWDINFO *awdi, double freqa4 );
VSLIB_IMPORT int __stdcall AS_AnalyzeWaveFile( char *wavfile, char *txtfile, int blockps, int nnoffset, int nnrange );


#endif /* _VSLIB_H_ */

