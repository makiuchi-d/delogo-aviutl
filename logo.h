/*====================================================================
*	ロゴパターン			logo.h
* 
* [ファイル構造]（暫定版）
* 
* 	"logo file x.xx\n"	// ファイルヘッダ文字列：バージョン情報とか(31byte)
* 	+----
* 	|	ファイルに含まれるロゴデータの数(1byte)
* 	+----
* 	|	LOGO_HEADER		// データヘッダ
* 	+----
* 	|
* 	:	LOGO_PIXEL[h*w]	// ピクセル情報：サイズはLOGO_HEADERのw,hから算出
* 	:
* 	+----
* 	|	LOGO_HEADER
* 	+----
* 	|
* 	:	LOGO_PIXEL[h*w]
* 	:
* 
*===================================================================*/
#ifndef ___LOGO_H
#define ___LOGO_H

/* ロゴファイルヘッダ
* 	31BYTEの文字列
* 	データ数 1BYTE
*/
#define LOGO_FILE_HEADER "<logo data file ver0 test3>\n\0\0\0\0\0"
#define LOGO_FILE_HEADER_STR_SIZE  31


/* ロゴデータ最大サイズ：4500pixelくらい
*	AviUtlの拡張データ領域は最大 57102 BYTE
*	これを超えるとプロファイル変更時などで落ちる
*/
#define LOGO_MAXPIXEL (4700)
#define LOGO_MAXSIZE  (57102)

/* 不透明度最大値 */
#define LOGO_MAX_DP   1000

/* ロゴ名最大文字数 */
#define LOGO_MAX_NAME 32

/*--------------------------------------------------------------------
*	LOGO_HEADER 構造体
*		ロゴの基本的な情報を記録
*-------------------------------------------------------------------*/
typedef struct {
	char     name[LOGO_MAX_NAME]; 	// 名称
	short    x, y;      			// 基本位置
	short    h, w;      			// ロゴ高さ・幅
	int      reserve[2];			// 拡張用に予約
} LOGO_HEADER;

/*--------------------------------------------------------------------
*	LOGO_PIXEL 構造体
*		ロゴの各ピクセルごとの情報を記録
*-------------------------------------------------------------------*/
typedef struct {
	short dp_y;		// 不透明度（輝度）
	short y;		// 輝度
	short dp_cb;	// 不透明度（青）
	short cb;		// 色差（青）
	short dp_cr;		// 不透明度（赤）
	short cr;		// 色差（赤）
} LOGO_PIXEL;


/*--------------------------------------------------------------------
*	ロゴデータ全体のサイズ
*-------------------------------------------------------------------*/
#define LOGO_DATASIZE(ptr)  \
	(sizeof(LOGO_HEADER)+((LOGO_HEADER *)ptr)->h*((LOGO_HEADER *)ptr)->w*sizeof(LOGO_PIXEL))

/*--------------------------------------------------------------------
*	ロゴデータのサイズ（ヘッダ無し）
*-------------------------------------------------------------------*/
#define LOGO_PIXELSIZE(ptr)  \
	(((LOGO_HEADER *)ptr)->h*((LOGO_HEADER *)ptr)->w*sizeof(LOGO_PIXEL))


#endif
