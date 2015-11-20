
//--------------------------------------------------------------
// VS4Uメイン
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
// メイン
//--------------------------------------------------------------
int mainproc( int argc, char *argv[] )
{
	// 概要表示
	if( argc <= 1 ){
		printf( "----------------------------------------\n");
		printf( "VS4U ver.1.32\n");
		printf( "Copyright (C) 2013-2014 あっきー\n");
		printf( "http://ackiesound.ifdef.jp/\n");
		printf( "----------------------------------------\n");
		return 0;
	}

#ifdef PRINT_TIME
	// 開始時間取得
	int time_start = GetTickCount();
#endif

	// オプション設定
	VS4UOPTION option;
	if( Vs4uGetOption( argc, argv, &option ) != 0 ){
		return 0;
	}

	// オプション表示
#ifdef PRINT_OPTION
	Vs4uPrintOption( &option);
#endif

	// ピッチ、フォルマント、タイムストレッチ
	if( Vs4uStep1( &option ) != 0 ){
		return 0;
	}

	// フィルタ、ボリューム
	if( Vs4uStep2( &option ) != 0 ){
		return 0;
	}

	return 0;
}

