
//--------------------------------------------------------------
// VS4U共通処理
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
// 実数値範囲制限
//
// 引数
//  int value	実数値
//  int min		最小値
//  int max		最大値
//
// 戻り値
//  範囲制限後の実数値
//
// 概要
//  最小値以下なら最小値に、最大値以上なら最大値に変換する。
//--------------------------------------------------------------
double LimitDoubleValue( double value, double min, double max )
{
	if( value < min ){
		value = min;
	}
	else if( value > max ){
		value = max;
	}
	return value;
}

//--------------------------------------------------------------
// 整数値範囲制限
// 
// 引数
//  int value	制限対象の値
//	int min		最小値
//	int max		最大値
// 
// 戻り値
//  範囲制限後の整数値
// 
// 概要
//  最小値以下なら最小値に、最大値以上なら最大値に変換する。
//--------------------------------------------------------------
int LimitIntValue( int value, int min, int max )
{
	if( value < min ){
		value = min;
	}
	else if( value > max ){
		value = max;
	}
	return value;
}


//--------------------------------------------------------------
// 拡張子付加
//
// 引数
//  char *dstStr	ファイル名
//  int dstSize		ファイル名バッファサイズ
//  char *extStr	拡張子
//
// 戻り値
//  なし
//
// 概要
//  ファイル名に拡張子を付加する。
//--------------------------------------------------------------
void AddExtStr( char *dstStr, int dstSize, char *extStr )
{
	int len = strlen( dstStr);

	// 元のファイル名の拡張子の「.」を「_」に変換
	for( int i=len-1; i>=0; i-- ){
		if( dstStr[i] == '\\' ){
			break;
		}
		if( dstStr[i] == '.' ){
			dstStr[i] = '_';
			break;
		}
	}
	
	// 拡張子付加
	strcat_s( dstStr, dstSize, extStr);
}


//--------------------------------------------------------------
// ファイル存在確認
//
// 引数
//  char *fileName	ファイル名
//
// 戻り値
//  ファイルがある:1,ファイルがない:0
//
// 概要
//  指定したファイルが存在すれば1を、ファイルが存在しなければ0を返す。
//--------------------------------------------------------------
int CheckFileExist( char *fileName )
{
	int existFlg = 0;
	FILE *fp;

	fopen_s( &fp, fileName, "rb");
	if ( fp != NULL ){
		fclose( fp);
		existFlg = 1;
	}

	return existFlg;
}

