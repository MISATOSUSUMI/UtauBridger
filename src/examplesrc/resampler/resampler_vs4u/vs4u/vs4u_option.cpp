
//--------------------------------------------------------------
// VS4Uオプション設定
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
// オプション設定
//
// 引数
//  int		argc		引数の数
//  char	*argv[]		引数
//  VS4UOPTION *option	オプション
//
// 戻り値
//  0:成功 1:失敗
//
// 概要
//  コマンドライン引数を読み出し、オプションを設定する。
//--------------------------------------------------------------
int Vs4uGetOption( int argc, char *argv[], VS4UOPTION *option )
{
#if 0
	printf( "argc = %d\n", argc);
	for ( int i=1; i<argc; i++ ){
		printf( "%d = %s\n", i, argv[i]);
	}
#endif

	// オプション初期化
	memset( option, 0, sizeof(VS4UOPTION));
	option->peekCmp = 86;
	option->breathiness = 50;

	// 1.入力ファイル
	strcpy_s( option->inputWaveFile, VS4U_MAX_PATH, argv[1]);

	// 2.出力ファイル
	strcpy_s( option->outputWaveFile, VS4U_MAX_PATH, argv[2]);

	// 3.音階名
	option->noteNum = Vs4uCnvNoteNum( argv[3]);

	// 4.元音長からの変化分(子音速度)
	option->fixSpeed = LimitDoubleValue( atof( argv[4] ), 0.0, 400.0);

	// 5.フラグ
	Vs4uSetFlgOption( argv[5], option);

	// 6.オフセット
	option->offset = atof( argv[6]);

	// 7.要求長
	option->reqLength = atof( argv[7]);

	// 8.子音部
	option->fixedLength = atof( argv[8]);

	// 9.ブランク
	option->endBlank = atof( argv[9]);

	// 10.ボリューム
	option->volume = LimitDoubleValue( atof( argv[10] ), 0.0, 200.0);

	// 11.モジュレーション
	option->modulation = LimitDoubleValue( atof( argv[11] ), -400.0, 400.0);

	// 12～13.ピッチベンド
	if( argc >= 13 ){
		Vs4uSetPitchBendOption( argv, option);
	}

	// 独自周波数表ファイル名設定
	strcpy_s( option->vsfrqFile, VS4U_MAX_PATH, option->inputWaveFile);
	AddExtStr( option->vsfrqFile, VS4U_MAX_PATH, ".vs4ufrq");

	// 標準周波数表ファイル名設定
	strcpy_s( option->frqFile, VS4U_MAX_PATH, option->inputWaveFile);
	AddExtStr( option->frqFile, VS4U_MAX_PATH, ".frq");

	// プロジェクトファイル名設定
	strcpy_s( option->vspFile, VS4U_MAX_PATH, option->outputWaveFile);
	AddExtStr( option->vspFile, VS4U_MAX_PATH, ".vshp");
	
	return 0;
}


//--------------------------------------------------------------
// 音階名文字列をノートナンバーに変換
//
// 引数
//  char *nnStr		音階名文字列(C0～B9)
//
// 戻り値
//  ノートナンバー(C4=60とする半音単位)
//
// 概要
//  音階名文字列をノートナンバーに変換する。
//--------------------------------------------------------------
int Vs4uCnvNoteNum( char *nnStr )
{
	int noteNum = 0;		// ノートナンバー
	int octStrOffset = 1;	// オクターブ文字のインデックス

	// 音階名文字列長をチェック
	int length = strlen( nnStr);
	if( length < 2 ){
		return 60;
	}

	// 音階名をノートナンバーに変換
	switch( nnStr[0] ){
	case 'C':
		noteNum = 0;
		break;
	case 'D':
		noteNum = 2;
		break;
	case 'E':
		noteNum = 4;
		break;
	case 'F':
		noteNum = 5;
		break;
	case 'G':
		noteNum = 7;
		break;
	case 'A':
		noteNum = 9;
		break;
	case 'B':
		noteNum = 11;
		break;
	default:
		noteNum = 0;
		break;
	}

	// #があれば半音上げる
	if( nnStr[1] == '#' ){
		noteNum++;
		octStrOffset = 2;
	}

	// オクターブを変換
	if( length > octStrOffset ){
		int oct = nnStr[octStrOffset]-'0';
		if( oct < 0 ){
			oct = 0;
		}
		else if( oct > 10 ){
			oct = 10;
		}
		noteNum += (oct+1)*12;
	}

	return noteNum;
}


//--------------------------------------------------------------
// フラグ文字列でオプション設定
//
// 引数
//  char *flgStr	フラグ文字列
//
// 戻り値
//  0:成功 1:失敗
//
// 概要
//  フラグ文字列でオプションを設定する。
//--------------------------------------------------------------
int Vs4uSetFlgOption( char *flgStr, VS4UOPTION *option )
{
	int length = strlen( flgStr);

	// 設定
	int offset = 0;	// 読み出し位置
	for(;;){
		// 終了判定
		if( offset == length ){
			break;
		}

		// フラグ種別読み出し
		char flg = flgStr[offset];
		offset++;

		// フラグ値の文字列長を算出
		int valueLength = 0;
		for(;;){
			// 終端に到達
			if( offset+valueLength == length ){
				break;
			}
			// 次のフラグ(アルファベット)に到達
			if( isalpha( flgStr[offset+valueLength] ) ){
				break;
			}
			valueLength++;
		}

		// フラグ値を数値に変換
		int valueData = 0;
		if( ( valueLength > 0 ) && ( valueLength < 32 ) ){
			char tmpStr[32];
			memcpy_s( tmpStr, 32, flgStr+offset, valueLength);
			tmpStr[valueLength] = 0;
			valueData = atoi( tmpStr);
			offset += valueLength;
		}

		// フラグ設定
		switch( flg ){
		case 'N':
			option->noFormantFlg = 1;
			break;
		case 'g':
			option->genderFactor = LimitIntValue( valueData, -100, 100);
			break;
		case 't':
			option->pitchShift = LimitIntValue( valueData, -240, 240);
			break;
		case 'T':
			option->pitchShift2 = LimitIntValue( valueData, -2400, 2400);
			break;
		case 'P':
			option->peekCmp = LimitIntValue( valueData, 0, 100);
			break;
		case 'H':
			option->lowPassfilter = LimitIntValue( valueData, 0, 99);
			break;
		case 'B':
			option->breathiness = LimitIntValue( valueData, 0, 100);
			break;
		case 'O':
			option->brightness = LimitIntValue( valueData, -100, 100);
			break;
		case 'S':
			option->clearness = LimitIntValue( valueData, -100, 100);
			break;
		case 'Z':
			option->outputVshpFlg = valueData;
			break;
#if 0
		case 'V':
			option->reGenerateFrqFlg = 1;
			break;
#endif
		case 'W':
			option->disablePitchShift = valueData;
			break;
		}
	}

	return 0;
}


//--------------------------------------------------------------
// Base64デコード
//
// 引数
//  char *str	文字列
//
// 戻り値
//  デコード値
//
// 概要
//  2桁のBASE64文字列をデコードし、-2048～2047の値を返す。
//--------------------------------------------------------------
int Vs4uDecodeBase64( char *str )
{
	int value = 0;

	for( int i=0; i<2; i++ ){
		int c = str[i];
		int v = 0;

		if( ( c >= 'A' ) && ( c <= 'Z' ) ){
			v = c-'A';
		}
		else if( ( c >= 'a' ) && ( c <= 'z' ) ){
			v = c-'a'+26;
		}
		else if( ( c >= '0' ) && ( c <= '9' ) ){
			v = c-'0'+52;
		}
		else if( c == '+' ){
			v = 62;
		}
		else if( c == '/' ){
			v = 63;
		}

		value = value*64+v;
	}

	if( value >= 2048 ){
		value -= 4096;
	}

	return value;
}


//--------------------------------------------------------------
// ピッチベンドオプション設定
//
// 引数
//  int		argc		引数の数
//  char	*argv[]		引数
//  VS4UOPTION *option	オプション
//
// 戻り値
//  0:成功 1:失敗
//
// 概要
//  コマンドライン引数を読み出し、ピッチベンドオプションを設定する。
//--------------------------------------------------------------
int Vs4uSetPitchBendOption( char *argv[], VS4UOPTION *option )
{
	char *pitchBendStr = argv[12];
	option->tempo = 0.0;
	
	// テンポ読み出し
	if( pitchBendStr[0] == '!' ){
		option->tempo = atof( pitchBendStr+1);
		pitchBendStr = argv[13];
	}

	// ピッチベンドデータ読み出し
	int length = strlen( pitchBendStr);	// 文字列長
	int nData = 0;		// データ数
	int offset = 0;		// 読み出し位置
	int lastData = 0;	// 前回のデータ(ランレングス処理用)

	for(;;){
		// 終了判定
		if( offset >= length ){
			break;
		}

		if( pitchBendStr[offset] == '#' ){
			// ランレングス文字列長算出
			int i = offset+1;
			int offset2 = i;
			for(;;){
				if( i == length ){
					break;
				}
				if( pitchBendStr[i] == '#' ){
					offset2 = i;
					break;
				}
				i++;
			}
			int repNumLength = offset2-offset-1;

			// ランレングス算出,データ設定
			int repNum = 0;
			if( repNumLength < 8 ){
				char tmp[8];
				memcpy_s( tmp, 8, pitchBendStr+offset+1, repNumLength);
				tmp[repNumLength] = NULL;
				repNum = atoi( tmp);
				for( int i=0; i<repNum; i++ ){
					if( nData+i < VS4U_PBSIZE ){
						option->pitchBend[nData+i] = lastData;
					}
					else{
						break;
					}
				}
			}

			nData += repNum;
			offset = offset2+1;
		}
		else{
			// 直接指定
			if( length >= offset+2 ){
				lastData = Vs4uDecodeBase64( pitchBendStr+offset);
				if( nData < VS4U_PBSIZE ){
					option->pitchBend[nData] = lastData;
				}
				nData++;
				offset += 2;
			}
			else{
				break;
			}
		}
	}

	// データ数設定
	if( nData > VS4U_PBSIZE ){
		nData = VS4U_PBSIZE;
	}
	option->nPitchBend = nData;

	return 0;
}


#ifdef PRINT_OPTION
//--------------------------------------------------------------
// オプション表示(デバッグ用)
//
// 引数
//  VS4UOPTION *option	オプション
//
// 戻り値
//  なし
//
// 概要
//  オプションを表示する。
//--------------------------------------------------------------
void Vs4uPrintOption( VS4UOPTION *option )
{
	printf( "----------------------------------------\n");

	printf( "入力ファイル:%s\n", option->inputWaveFile);
	printf( "出力ファイル:%s\n", option->outputWaveFile);
	printf( "標準周波数表:%s\n", option->frqFile);
	printf( "独自周波数表:%s\n", option->vsfrqFile);

	printf( "出力音階:%d\n", option->noteNum);
	printf( "オフセット:%f\n", option->offset);
	printf( "要求長:%f\n", option->reqLength);
	printf( "子音部:%f\n", option->fixedLength);
	printf( "ブランク:%f\n", option->endBlank);
	printf( "子音速度:%f\n", option->fixSpeed);
	
	printf( "ボリューム:%f\n", option->volume);
	printf( "モジュレーション:%f\n", option->modulation);

	printf( "フォルマント調整なしフラグ:%d\n", option->noFormantFlg);
	printf( "ジェンダーファクター:%d\n", option->genderFactor);
	printf( "周波数表再作成フラグ:%d\n", option->reGenerateFrqFlg);
	printf( "ピッチ微調整:%d\n", option->pitchShift);

	printf( "ピッチベンドデータ数:%d\n", option->nPitchBend);
	printf( "テンポ:%f\n", option->tempo);

#if 0
	printf( "ピッチベンドデータ:");
	for( int i=0; i<option->nPitchBend; i++ ){
		printf( "%d,", option->pitchBend[i]);
	}
	printf( "\n");
#endif

	printf( "----------------------------------------\n");
}
#endif // PRINT_OPTION
