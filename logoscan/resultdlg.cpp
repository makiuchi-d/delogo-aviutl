/*====================================================================
* 	解析結果ダイアログ			resultdlg.cpp
*===================================================================*/
#include <windows.h>
#include "..\filter.h"
#include "..\logo.h"
#include "resultdlg.h"
#include "resource.h"
#include "..\send_lgd.h"
#include "xywh.h"


#define LGD_FILTER  "ロゴデータ (*.lgd)\0*.lgd\0全てのファイル (*.*)\0*.*\0"
#define LGD_DEFAULT "*.lgd"


FILTER* dlgfp;	// FILTER構造体
char    defname[32];	// デフォルトロゴ名

PIXEL* pix;	// 表示用ビットマップ
BITMAPINFO  bmi;
const PIXEL_YC yc_black = {    0,    0,    0 };	// 黒
const PIXEL_YC yc_white = { 4080,    0,    0 };	// 白
const PIXEL_YC yc_red   = { 1220, -688, 2040 };	// 赤
const PIXEL_YC yc_green = { 2393,-1351,-1707 };	// 緑
const PIXEL_YC yc_blue  = {  467, 2040, -333 };	// 青


UINT WM_SEND_LOGO_DATA;	// ロゴデータ送信メッセージ
FILTER* delogofp;	// ロゴ消しフィルタFILTER構造体


extern void* logodata;	// ロゴデータ（解析結果）[filter.cpp]
extern char  filter_name[];	// フィルタ名 [filter.cpp]

//----------------------------
// 	プロトタイプ
//----------------------------
static void Wm_initdialog(HWND hdlg);
static void DispLogo(HWND hdlg,const PIXEL_YC ycbg);
static void idc_save(HWND hdlg);
static void ExportLogoData(char *fname,void *data,HWND hdlg);
static void SendLogoData(HWND hdlg);

/*====================================================================
* 	OptDlgProc()		コールバックプロシージャ
*===================================================================*/
BOOL CALLBACK ResultDlgProc(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg){
		case WM_INITDIALOG:
			Wm_initdialog(hdlg);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDCANCEL:
				case IDC_CLOSE:	// 閉じるボタン
					if(pix) VirtualFree(pix,0,MEM_RELEASE);
					EndDialog(hdlg,LOWORD(wParam));
					break;

				case IDC_SAVE:	// 保存ボタン
					idc_save(hdlg);
					break;

				case IDC_SEND:	// 送信ボタン
					SendLogoData(hdlg);
					break;

				//--------------------------------背景色設定
				case IDC_BLACK:
					DispLogo(hdlg,yc_black);
					return TRUE;

				case IDC_WHITE:
					DispLogo(hdlg,yc_white);
					return TRUE;

				case IDC_RED:
					DispLogo(hdlg,yc_red);
					return TRUE;

				case IDC_GREEN:
					DispLogo(hdlg,yc_green);
					return TRUE;

				case IDC_BLUE:
					DispLogo(hdlg,yc_blue);
					return TRUE;
			}
			break;
	}

	return FALSE;
}


/*--------------------------------------------------------------------
* 	Wm_initdialog()		初期化
*-------------------------------------------------------------------*/
static void Wm_initdialog(HWND hdlg)
{
	// 最大文字数セット
	SendDlgItemMessage(hdlg,IDC_EDIT,EM_LIMITTEXT,LOGO_MAX_NAME-2,0);
	// デフォルトロゴ名セット
	SetDlgItemText(hdlg,IDC_EDIT,defname);

	pix = NULL;

	// ロゴ消しフィルタを探す
	for(int n=0;(delogofp=(FILTER*)dlgfp->exfunc->get_filterp(n))!=NULL;n++){
		if(lstrcmp(delogofp->name,LOGO_FILTER_NAME)==0){	// 名前で判別
			// ロゴ消しフィルタが見つかった
			WM_SEND_LOGO_DATA = RegisterWindowMessage(wm_send_logo_data);
			return;
		}
	}

	// みつからなかった時
	delogofp = NULL;
	EnableWindow(GetDlgItem(hdlg,IDC_SEND),FALSE);	// 送信禁止
}

/*--------------------------------------------------------------------
* 	DispLogo()	ロゴを表示
*-------------------------------------------------------------------*/
static void DispLogo(HWND hdlg,const PIXEL_YC ycbg)
{
	LOGO_HEADER *lgh;
	LOGO_PIXEL  *lgp;
	PIXEL_YC    yc;
	int   i,j,t;
	HDC   hdc;
	HWND  panel;
	RECT  rec;

	lgh = (LOGO_HEADER*)logodata;

	// BITMAPINFO設定
	ZeroMemory(&bmi,sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = lgh->w + (4-lgh->w%4);	// ４の倍数
	bmi.bmiHeader.biHeight      = lgh->h;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	// メモリ再確保
	pix = (PIXEL*)VirtualAlloc(pix,bmi.bmiHeader.biWidth*bmi.bmiHeader.biHeight*sizeof(PIXEL)
																	,MEM_COMMIT,PAGE_READWRITE);
	if(pix==NULL){	// メモリ確保失敗
		MessageBox(hdlg,"メモリが確保できませんでした\nDispLogo()",filter_name,MB_OK|MB_ICONERROR);
		return;	// 何もしない
	}

	(void *)lgp = lgh + 1;

	// RGBデータ作成
	for(i=0;i<lgh->h;i++){
		for(j=0;j<lgh->w;j++){
			// 輝度
			yc.y = ((long)ycbg.y*((long)LOGO_MAX_DP-lgp->dp_y) + (long)lgp->y*lgp->dp_y) / LOGO_MAX_DP;
			// 色差(青)
			yc.cb = ((long)ycbg.cb*((long)LOGO_MAX_DP-lgp->dp_cb) + (long)lgp->cb*lgp->dp_cb) / LOGO_MAX_DP;
			// 色差(赤)
			yc.cr = ((long)ycbg.cr*((long)LOGO_MAX_DP-lgp->dp_cr) + (long)lgp->cr*lgp->dp_cb) / LOGO_MAX_DP;

			// YCbCr -> RGB
			dlgfp->exfunc->yc2rgb(&pix[bmi.bmiHeader.biWidth*(lgh->h-1-i)+j],&yc,1);

			lgp++;
		}
	}

	// ウィンドウハンドル取得
	panel = GetDlgItem(hdlg,IDC_PANEL);

	// rect設定
	GetClientRect(panel,&rec);
	rec.left  = 2;
	rec.top   = 8;
	rec.right  -= 3;
	rec.bottom -= 3;

	// 表示画像の倍率・位置
	double magnify;	// 倍率

	if(rec.right-rec.left >= lgh->w*2){	// 幅が収まる時
		if(rec.bottom-rec.top >= lgh->h*2)	// 高さも収まる時
			magnify = 2;
		else	// 高さのみ収まらない
			magnify = ((double)rec.bottom-rec.top)/lgh->h;
	}
	else {
		if(rec.bottom-rec.top >= lgh->h*2)	// 幅のみ収まらない
			magnify = ((double)rec.right-rec.left)/lgh->w;
		else{ // 幅も高さも収まらない
			magnify = ((double)rec.bottom-rec.top)/lgh->h;// 高さで計算
			magnify = (magnify>((double)rec.right-rec.left)/lgh->w) ? // 倍率が小さい方
								((double)rec.right-rec.left)/lgh->w : magnify;
		}
	}

	i = (rec.right-rec.left - lgh->w*magnify)/2 + rec.left;	// 中央に表示するように
	j = (rec.bottom-rec.top - lgh->h*magnify)/2 + rec.top;	// left,topを計算

	// デバイスコンテキスト取得
	hdc = GetDC(panel);

	SetStretchBltMode(hdc,COLORONCOLOR);
	// 2倍に拡大表示
	StretchDIBits(hdc,i,j,lgh->w*magnify,lgh->h*magnify,
					0,0,lgh->w,lgh->h,pix,&bmi,DIB_RGB_COLORS,SRCCOPY);

	ReleaseDC(panel,hdc);
}

/*--------------------------------------------------------------------
* 	SaveLogoData
*-------------------------------------------------------------------*/
static void idc_save(HWND hdlg)
{
	char filename[MAX_PATH];

	ZeroMemory(defname,32);
	GetDlgItemText(hdlg,IDC_EDIT,defname,LOGO_MAX_NAME);
	if(lstrlen(defname)==0){
		MessageBox(hdlg,"ロゴ名を入力してください",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	// ロゴ名設定
	defname[LOGO_MAX_NAME-1] = '\0';	// 終端
	lstrcpy((char*)logodata,defname);

	// 保存ファイル名取得
	wsprintf(filename,"%s.lgd",defname);
	if(!dlgfp->exfunc->dlg_get_save_name(filename,LGD_FILTER,filename))
		return;

	ExportLogoData(filename,logodata,hdlg);
}

/*--------------------------------------------------------------------
* 	ExportLogoData()	ロゴデータを書き出す
*-------------------------------------------------------------------*/
static void ExportLogoData(char *fname,void *data,HWND hdlg)
{
	HANDLE hFile;
	DWORD  dw;
	DWORD  size;
	char   c =1;
	int    s =0;

	// ファイルを開く
	hFile = CreateFile(fname,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==NULL){
		MessageBox(hdlg,"ファイルのオープンに失敗しました",filter_name,MB_OK|MB_ICONERROR);
	}
	SetFilePointer(hFile,0, 0, FILE_BEGIN);	// 先頭へ

	// ヘッダ書き込み
	dw = 0;
	WriteFile(hFile,LOGO_FILE_HEADER,31,&dw,NULL);
	if(dw!=31){	// 書き込み失敗
		MessageBox(hdlg,"ロゴデータ保存に失敗しました(1)",filter_name,MB_OK|MB_ICONERROR);
		s=1;
	}
	else{	// 成功
		// データ数書き込み（必ず１）
		dw = 0;
		WriteFile(hFile,&c,1,&dw,NULL);
		if(dw!=1){	// 書き込み失敗
			MessageBox(hdlg,"ロゴデータ保存に失敗しました(2)",filter_name,MB_OK|MB_ICONERROR);
			s=1;
		}
		else{	// 成功
			// データ書き込み
			size = LOGO_DATASIZE(data);	// データサイズ取得
			dw = 0;
			WriteFile(hFile,data,size,&dw,NULL);
			if(dw!=size){
				MessageBox(hdlg,"ロゴデータ保存に失敗しました(3)",filter_name,MB_OK|MB_ICONERROR);
				s=1;
			}
		}
	}

	CloseHandle(hFile);

	if(s)	// エラーがあったとき
		DeleteFile(fname);	// ファイル削除
}

/*--------------------------------------------------------------------
* 	SendLogoData()	ロゴデータを送信する
*-------------------------------------------------------------------*/
static void SendLogoData(HWND hdlg)
{
	if(!delogofp) return;	// ロゴ消しフィルタが無い
	if(!logodata) return;	// ロゴデータが無い

	// ロゴ名設定
	ZeroMemory(defname,32);
	GetDlgItemText(hdlg,IDC_EDIT,defname,LOGO_MAX_NAME);
	if(lstrlen(defname)==0){
		MessageBox(hdlg,"ロゴ名を入力してください",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	defname[LOGO_MAX_NAME-1] = '\0';	// 終端
	lstrcpy((char*)logodata,defname);

	SendMessage(delogofp->hwnd,WM_SEND_LOGO_DATA,(WPARAM)logodata,0);
}
