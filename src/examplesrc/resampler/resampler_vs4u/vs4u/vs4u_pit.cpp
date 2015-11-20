
//--------------------------------------------------------------
// VS4U ピッチ、フォルマント、タイムストレッチ
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#include "vslib.h"
#include "vs4u.h"


//--------------------------------------------------------------
// ピッチシフト、フォルマントシフト、タイムストレッチ
//
// 引数
//  VS4UOPTION *option	オプション
//
// 戻り値
//  0:成功 1:失敗
//
// 概要
//  ピッチシフト、フォルマントシフト、タイムストレッチ処理を行う。
//  (inputWaveFile→tmpWaveData)
//--------------------------------------------------------------
int Vs4uStep1( VS4UOPTION *option )
{
	VSPRJINFO vsPrjInfo;
	VSITEMINFO vsItemInfo;

	// プロジェクト作成
	HVSPRJ hVsprj = Vs4uLoadProject( option);
	if( hVsprj == NULL )
	{
		return 1;
	}

	// タイムストレッチ設定
	Vs4uSetTimeStretchPrm( hVsprj, option);

	// ピッチ設定
	Vs4uSetPitPrm( hVsprj, option);

	// 音声合成方式設定
	if( option->disablePitchShift == -2 )
	{
		// 和音モードに設定
		VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
		vsItemInfo.synthMode = SYNTHMODE_P;
		VslibSetItemInfo( hVsprj, 0, &vsItemInfo);
	}
	else
	{
		if( option->noFormantFlg != 0 )
		{
			// フォルマント調整なしなら単音モードに設定
			VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
			vsItemInfo.synthMode = SYNTHMODE_M;
			VslibSetItemInfo( hVsprj, 0, &vsItemInfo);
		}
		else
		{
			// フォルマント補正ありなら単音フォルマント補正モードに設定
			VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
			vsItemInfo.synthMode = SYNTHMODE_MF;
			VslibSetItemInfo( hVsprj, 0, &vsItemInfo);
		}
	}

	// EQ設定
	int eq1[15] = { 0, 0, 0, 0, 0, 0, 0, 20, 60, 100, 120, 120, 80,   0,   0 };
	int eq2[15] = { 0, 0, 0, 0, 0, 0, 0,  0,  0,   0,   0,   0, 40, 120, 120 };
	VslibSetEQGain( hVsprj, 0, 0, eq1);
	VslibSetEQGain( hVsprj, 0, 1, eq2);

	// 制御点数取得
	VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
	int nCtrlPnt = vsItemInfo.ctrlPntNum;
	
	// 制御点パラメータ設定
	for( int i=0; i<nCtrlPnt; i++ ){
		// 制御点情報取得
		VSCPINFOEX vsCpInfo;
		VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
		
		// ジェンダーファクター設定
		vsCpInfo.formant = -20*option->genderFactor;

		// ブレシネス設定
		vsCpInfo.breathiness = (option->breathiness-50)*200;

		// ブライトネス設定
		vsCpInfo.eq1 = option->brightness*100;

		// クリアネス設定
		vsCpInfo.eq2 = option->clearness*100;

		// 制御点情報設定
		VslibSetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
	}

	// サンプリング周波数、サンプル数取得
	VslibGetProjectInfo( hVsprj, &vsPrjInfo);
	option->tmpSmpFreq = vsPrjInfo.sampFreq;
	VslibGetMixSample( hVsprj, &option->tmpSample);

	// WAVEデータ取得
	option->tmpWaveData = (short *)calloc( option->tmpSample, sizeof(short));
	VslibGetMixData( hVsprj, option->tmpWaveData, 16, 1, 0, option->tmpSample);

	// プロジェクトファイル出力
	if( option->outputVshpFlg == 12345 ){
		VslibSaveProject( hVsprj, option->vspFile);
	}

	// プロジェクト削除
	VslibDeleteProject( hVsprj);

	return 0;
}


//--------------------------------------------------------------
// プロジェクト作成
//
// 引数
//  VS4UOPTION *option	オプション
//
// 戻り値
//  プロジェクトハンドル
//
// 概要
//  独自周波数表を読み込み、プロジェクトハンドルを返す。
//  独自周波数表がない場合は作成する。
//  失敗した場合はNULLを返す。
//--------------------------------------------------------------
HVSPRJ Vs4uLoadProject( VS4UOPTION *option )
{
	int sta;

	// WAVEファイル存在チェック
	if( CheckFileExist( option->inputWaveFile ) == 0 ){
		printf( "VS4U:入力ファイルが見つかりません.\n");
		return NULL;
	}

	// 標準周波数表存在チェック
	int existFrqFlg = CheckFileExist( option->frqFile);

	// 独自周波数表存在チェック
	int existVsfrqFlg = CheckFileExist( option->vsfrqFile);

	// プロジェクト作成 ----------------------------------------
	HVSPRJ hVsprj = NULL;

	if( option->reGenerateFrqFlg != 0 ){
		existVsfrqFlg = 0;
	}
	else{
		if( existVsfrqFlg != 0 ){
			// 独自周波数表を読み込む
			sta = VslibOpenProject( &hVsprj, option->vsfrqFile);
			if( sta != VSERR_NOERR ){
				printf( "VS4U:周波数表読み込みエラー.\n");
				existVsfrqFlg = 0;
			}
		}
	}

	if( existVsfrqFlg == 0 ){
		// 独自周波数表(vsp)を作成する
		printf( "VS4U:周波数表を作成します.\n");

		// プロジェクト作成
		sta = VslibCreateProject( &hVsprj);
		if( sta != VSERR_NOERR ){
			printf( "VS4U:初期化エラー.\n");
			return NULL;
		}
		
		// アイテム追加
		int itemNum;
		sta = VslibAddItemEx( hVsprj, option->inputWaveFile, &itemNum,  36, 48, 0);
		if( sta != VSERR_NOERR ){
			printf( "VS4U:ファイル入力エラー.\n");
			return NULL;
		}

		// 標準周波数表を利用して周波数設定
		if( existFrqFlg != 0 ){
			Vs4uLoadFrqFile( hVsprj, option);
		}

		// 独自周波数表保存
		sta = VslibSaveProject( hVsprj, option->vsfrqFile);
		if( sta != VSERR_NOERR ){
			printf( "VS4U:周波数表の保存に失敗しました.\n");
		}
		else{
			printf( "VS4U:周波数表を作成しました.\n");
		}
	}

	return hVsprj;
}


//--------------------------------------------------------------
// 標準周波数表を利用して周波数設定
//
// 引数
//  HVSPRJ hVsprj	プロジェクト
//  VS4UOPTION *option	オプション
//
// 戻り値
//  0:成功 1:失敗
//
// 概要
//  標準周波数表を利用してピッチ解析値を設定する。
//--------------------------------------------------------------
int Vs4uLoadFrqFile( HVSPRJ hVsprj, VS4UOPTION *option )
{
	VSPRJINFO vsPrjInfo;
	VSITEMINFO vsItemInfo;
	int retVal = 0;

	FILE *fpFrq;
	fopen_s( &fpFrq, option->frqFile, "rb");

	if ( fpFrq != NULL ){
		// データ間隔[サンプル]
		int itvSmp = 256;
		fseek( fpFrq, 8, SEEK_SET);
		fread( &itvSmp, sizeof(int), 1, fpFrq);
		
		// キー周波数
		double keyFreq = 440.0;
		fseek( fpFrq, 12, SEEK_SET);
		fread( &keyFreq, sizeof(double), 1, fpFrq);

		// データ数
		int nData = 0;
		fseek( fpFrq, 36, SEEK_SET);
		fread( &nData, sizeof(int), 1, fpFrq);

		// 周波数データ
		FREQDATA *freqData = (FREQDATA *)calloc( nData, sizeof(FREQDATA));
		fseek( fpFrq, 40, SEEK_SET);
		fread( freqData, sizeof(FREQDATA), nData, fpFrq);

#if 0
		// 表示(デバッグ用)
		printf( "itvSmp=%d\n", itvSmp);
		printf( "keyFreq=%f\n", keyFreq);
		printf( "nData=%d\n", nData);
		for( int i=0; i<nData; i++ ){
			printf( "%f,%f; ", freqData[i].freq, freqData[i].dyn);
		}
		getchar();
#endif

		// 制御点数取得
		VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
		int nCtrlPnt = vsItemInfo.ctrlPntNum;

		// サンプリング周波数取得
		VslibGetProjectInfo( hVsprj, &vsPrjInfo);
		int sampFreq = vsPrjInfo.sampFreq;
		double cpSec = 1.0/vsItemInfo.ctrlPntPs;	// 制御点あたりの秒数

		// 周波数設定
		for( int i=0; i<nCtrlPnt; i++ ){
			// 標準周波数表上でのポイント算出
			double smp = i*cpSec*sampFreq;
			int pnt = (int)(smp/itvSmp);

			// 周波数設定
			double freq = 0.0;
			if( ( pnt >= 0 ) && ( pnt < nData ) ){
				freq = freqData[pnt].freq;
			}

			if( freq > 55.0 ){	// ピッチあり
				// 編集前ピッチとピッチ有無フラグを設定
				VSCPINFOEX vsCpInfo;
				VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
				vsCpInfo.pitOrg = (int)(log(freq/440.0)/log(2.0)*1200.0)+6900;
				vsCpInfo.pitFlgEdit = 1;
				VslibSetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
			}
			else{				// ピッチなし
				// 編集前ピッチとピッチ有無フラグを設定
				VSCPINFOEX vsCpInfo;
				VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
				vsCpInfo.pitOrg = (int)(log(keyFreq/440.0)/log(2.0)*1200.0)+6900;
				vsCpInfo.pitFlgEdit = 0;
				VslibSetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
			}
		}
		
		// 周波数データ解放
		free( freqData);
		freqData = NULL;

		// ファイルを閉じる
		fclose( fpFrq);
	}
	else{
		retVal = 1;
	}

	return retVal;
}


//--------------------------------------------------------------
// タイムストレッチ設定
//
// 引数
//  HVSPRJ hVsprj	プロジェクト
//  VS4UOPTION *option	オプション
//
// 戻り値
//  0:成功
//
// 概要
//  タイムストレッチパラメータを設定する。
//--------------------------------------------------------------
int Vs4uSetTimeStretchPrm( HVSPRJ hVsprj, VS4UOPTION *option )
{
	VSPRJINFO vsPrjInfo;
	VSITEMINFO vsItemInfo;

	// サンプリング周波数取得
	VslibGetProjectInfo( hVsprj, &vsPrjInfo);
	int sampFreq = vsPrjInfo.sampFreq;

	// アイテム情報取得
	VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
	double orgSmp = vsItemInfo.sampleOrg;					// 元のサンプル数
	double offsetSmp = option->offset*0.001*sampFreq;		// オフセットのサンプル数
	double fixedSmp = option->fixedLength*0.001*sampFreq;	// 子音部のサンプル数
	double endBlankSmp = option->endBlank*0.001*sampFreq;	// ブランクのサンプル数
	double reqSmp = option->reqLength*0.001*sampFreq;		// 要求長のサンプル数

	// ブランクが負(オフセットからの相対値)なら終端からのブランクに変換する
	if( endBlankSmp < 0 ){
		endBlankSmp = orgSmp-offsetSmp+endBlankSmp;
	}

	// 子音部のストレッチ倍率
	double fixStretch = pow( 2.0, 1.0-option->fixSpeed/100.0);

	// タイミング制御点追加
	if( reqSmp >= fixedSmp*fixStretch ){
		// 要求長が子音部以上
		VslibSetTimeCtrlPnt( hVsprj, 0, 1, (int)orgSmp, (int)reqSmp);
		VslibAddTimeCtrlPnt( hVsprj, 0, (int)(orgSmp-endBlankSmp), (int)reqSmp);
		VslibAddTimeCtrlPnt( hVsprj, 0, (int)(offsetSmp+fixedSmp), (int)(fixedSmp*fixStretch));
		VslibAddTimeCtrlPnt( hVsprj, 0, (int)offsetSmp, 0);
	}
	else{
		// 要求長が子音部未満
		VslibSetTimeCtrlPnt( hVsprj, 0, 1, (int)orgSmp, (int)reqSmp);
		VslibAddTimeCtrlPnt( hVsprj, 0, (int)(offsetSmp+reqSmp/fixStretch), (int)reqSmp);
		VslibAddTimeCtrlPnt( hVsprj, 0, (int)offsetSmp, 0);
	}

	return 0;
}


//--------------------------------------------------------------
// 母音部分のピッチ平均値計算
//
// 引数
//  HVSPRJ hVsprj	プロジェクト
//  VS4UOPTION *option	オプション
//
// 戻り値
//  0:成功
//
// 概要
//  母音部分のピッチ平均値計算を計算する。
//--------------------------------------------------------------
int Vs4uCalcAveragePit( HVSPRJ hVsprj, VS4UOPTION *option )
{
	VSITEMINFO vsItemInfo;

	// 制御点数取得
	VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
	int nCtrlPnt = vsItemInfo.ctrlPntNum;

	// 制御点あたりの秒数を算出
	double ctrpntSec = 1.0/vsItemInfo.ctrlPntPs;

	// 母音区間の制御点番号を算出
	int startPnt = (int)((option->offset+option->fixedLength)*0.001/ctrpntSec);
	int endPnt = (int)(nCtrlPnt-option->endBlank*0.001/ctrpntSec);
	startPnt = LimitIntValue( startPnt, 0, nCtrlPnt);
	endPnt = LimitIntValue( endPnt, 0, nCtrlPnt);

	// 母音のピッチ平均値算出
	int pitSum = 0;
	int count = 0;
	for( int i=startPnt; i<endPnt; i++ ){
		VSCPINFOEX vsCpInfo;
		VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
		if( vsCpInfo.pitFlgEdit == 1 ){	// ピッチがある部分のみカウント
			pitSum += vsCpInfo.pitOrg;
			count++;
		}
	}
	int pitAverage = 6000;
	if( count != 0 ){
		pitAverage = pitSum/count;
	}

	// 母音のピッチ平均値算出(2回目)
	pitSum = 0;
	count = 0;
	for( int i=startPnt; i<endPnt; i++ ){
		VSCPINFOEX vsCpInfo;
		VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
		if( vsCpInfo.pitFlgEdit == 1 ){	// ピッチがある部分のみカウント
			if( abs(vsCpInfo.pitOrg-pitAverage) < 200 ){
				pitSum += vsCpInfo.pitOrg;
				count++;
			}
		}
	}
	pitAverage = 6000;
	if( count != 0 ){
		pitAverage = pitSum/count;
	}

	return pitAverage;
}


//--------------------------------------------------------------
// ピッチ設定
//
// 引数
//  HVSPRJ hVsprj	プロジェクト
//  VS4UOPTION *option	オプション
//
// 戻り値
//  0:成功
//
// 概要
//  ピッチパラメータを設定する。
//--------------------------------------------------------------
int Vs4uSetPitPrm( HVSPRJ hVsprj, VS4UOPTION *option )
{
	VSPRJINFO vsPrjInfo;
	VSITEMINFO vsItemInfo;

	// サンプリング周波数取得
	VslibGetProjectInfo( hVsprj, &vsPrjInfo);
	int sampFreq = vsPrjInfo.sampFreq;

	// 制御点数取得
	VslibGetItemInfo( hVsprj, 0, &vsItemInfo);
	int nCtrlPnt = vsItemInfo.ctrlPntNum;

	// 制御点あたりの秒数を計算
	double ctrpntSec = 1.0/vsItemInfo.ctrlPntPs;

	// ピッチ平均値を計算
	int pitAverage = Vs4uCalcAveragePit( hVsprj, option);

	// 全体にかかるピッチシフト量を計算
	int globalShift = 10*option->pitchShift+option->pitchShift2;
	
	// ピッチベンド値の間隔[秒単位]を計算
	double pbitvSec = 256.0/sampFreq;
	if( option->tempo > 0.0 ){
		pbitvSec = 60.0/option->tempo/96;	// 4分音符の1/96の秒数
	}

	if( ( option->disablePitchShift == -1 ) || ( option->disablePitchShift == -2 ) )
	{
		for( int i=0; i<nCtrlPnt; i++ ){
			// ピッチ設定
			VSCPINFOEX vsCpInfo;
			VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
			vsCpInfo.pitEdit = vsCpInfo.pitOrg;
			VslibSetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
		}
	}
	else
	{
		// 編集後ピッチパラメータ設定 ------------------------------
		for( int i=0; i<nCtrlPnt; i++ ){
			// 編集後の位置(秒単位)計算
			double edtSec;
			VslibGetStretchEditSec( hVsprj, 0, i*ctrpntSec, &edtSec);
		
			// ピッチベンド値計算
			int bendPnt = (int)(edtSec/pbitvSec);
			int bend = 0;
			if( ( bendPnt >= 0 ) && ( bendPnt < option->nPitchBend ) ){
				bend = option->pitchBend[bendPnt];
			}

			// ピッチ設定
			VSCPINFOEX vsCpInfo;
			VslibGetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
			int modPit = (int)((vsCpInfo.pitOrg-pitAverage)*option->modulation/100.0);
			vsCpInfo.pitEdit = option->noteNum*100+modPit+bend+globalShift;
			VslibSetCtrlPntInfoEx( hVsprj, 0, i, &vsCpInfo);
		}

	
		// 編集後ピッチパラメータ(VS4U専用)設定 --------------------
		// 出力サンプル数取得
		int mixSample;
		VslibGetMixSample( hVsprj, &mixSample);

		// ピッチベンドデータ数を算出
		double mixSec = (double)mixSample/sampFreq;
		int nPitchBend = (int)(mixSec/pbitvSec)+1;

		// ピッチベンドデータをピッチ変更量[cent]に変換
		for( int i=0; i<nPitchBend; i++ ){
			// ピッチベンドデータの位置に対応する制御点番号を計算
			double orgSec;
			double edtSec = LimitDoubleValue( i*pbitvSec, 0.0, mixSec);
			VslibGetStretchOrgSec( hVsprj, 0, edtSec, &orgSec);
			int pnt = LimitIntValue( (int)(orgSec/ctrpntSec), 0, nCtrlPnt-1);

			// 編集前ピッチを取得
			VSCPINFOEX vsCpInfo;
			VslibGetCtrlPntInfoEx( hVsprj, 0, pnt, &vsCpInfo);
			int pitOrg = vsCpInfo.pitOrg;

			// 編集後ピッチを計算
			int modPit = (int)((pitOrg-pitAverage)*option->modulation/100.0);
			int pitEdt = option->noteNum*100+modPit+globalShift+option->pitchBend[i];

			// ピッチ変更量をセット
			option->pitchBend[i] = pitEdt-pitOrg;
		}

		// ピッチ変更量データを設定
		VslibSetPitchArray( hVsprj, 0, option->pitchBend, nPitchBend, pbitvSec*sampFreq);
	}

	return 0;
}

