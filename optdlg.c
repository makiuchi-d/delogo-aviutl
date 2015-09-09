/*====================================================================
* 	オプションダイアログ			optdlg.c
*===================================================================*/
#include <windows.h>
#include <commctrl.h>
#include "filter.h"
#include "logo.h"
#include "optdlg.h"
#include "resource.h"


#define LGD_FILTER  "ロゴデータファイル (*.lgd)\0*.lgd\0"\
                    "全てのファイル (*.*)\0*.*\0"
#define LGD_DEFAULT "*.lgd"


//----------------------------
//	関数プロトタイプ
//----------------------------
static void Wm_initdialog(HWND hdlg);
static BOOL on_IDOK(HWND,WPARAM);
static BOOL on_IDCANCEL(HWND,WPARAM);
static void on_IDC_ADD(HWND hdlg);
static void on_IDC_DEL(HWND hdlg);
static void on_IDC_EXPORT(HWND hdlg);
static void on_IDC_UP(HWND hdlg);
static void on_IDC_DOWN(HWND hdlg);

static void AddItem(HWND hdlg,void *data);
static void DeleteItem(HWND list,int num);
static int  ReadLogoData(char *fname,HWND hdlg);
static void ExportLogoData(char *,void *,HWND);
static void CopyLBtoCB(HWND list,HWND combo);
static void CopyCBtoLB(HWND combo,HWND list);
static void DispLogo(HWND hdlg);


//----------------------------
//	グローバル変数
//----------------------------
FILTER *optfp;
HWND    hcb_logo;	// コンボボックスのハンドル
HWND    hoptdlg;
PIXEL  *pix;		// 表示用ピクセル
PIXEL_YC bgyc; // 背景色
const PIXEL_YC yc_black = {    0,    0,    0 };	// 黒
const PIXEL_YC yc_white = { 4080,    0,    0 };	// 白
const PIXEL_YC yc_red   = { 1220, -688, 2040 };	// 赤
const PIXEL_YC yc_green = { 2393,-1351,-1707 };	// 緑
const PIXEL_YC yc_blue  = {  467, 2040, -333 };	// 青

void **add_list;
void **del_list;
int  add_num,del_num;
int  add_buf,del_buf;

extern char filter_name[];



/*====================================================================
* 	OptDlgProc()		コールバックプロシージャ
*===================================================================*/
BOOL CALLBACK OptDlgProc(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg){
		case WM_INITDIALOG:
			Wm_initdialog(hdlg);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDOK:
					return on_IDOK(hdlg,wParam);

				case IDCANCEL:
					return on_IDCANCEL(hdlg,wParam);

				case IDC_ADD:
					on_IDC_ADD(hdlg);
					break;

				case IDC_DEL:
					on_IDC_DEL(hdlg);
					break;

				case IDC_EXPORT:
					on_IDC_EXPORT(hdlg);
					break;

				case IDC_UP:
					on_IDC_UP(hdlg);
					break;

				case IDC_DOWN:
					on_IDC_DOWN(hdlg);
					break;

				//------------------------------------------背景色変更
				case IDC_BLACK:
					bgyc = yc_black;
					DispLogo(hdlg);
					return TRUE;

				case IDC_WHITE:
					bgyc = yc_white;
					DispLogo(hdlg);
					return TRUE;

				case IDC_RED:
					bgyc = yc_red;
					DispLogo(hdlg);
					return TRUE;

				case IDC_GREEN:
					bgyc = yc_green;
					DispLogo(hdlg);
					return TRUE;

				case IDC_BLUE:
					bgyc = yc_blue;
					DispLogo(hdlg);
					return TRUE;

				//--------------------------------------プレビュー処理
				case IDC_LIST:
					switch(HIWORD(wParam)){
						case LBN_SELCHANGE:
							DispLogo(hdlg);
							break;
					}
					break;
			}
	}

	return FALSE;
}

/*--------------------------------------------------------------------
* 	Wm_initdialog()	初期化
*-------------------------------------------------------------------*/
static void Wm_initdialog(HWND hdlg)
{
	hoptdlg = hdlg;
	add_list = malloc(4*sizeof(void *));
	del_list = malloc(4*sizeof(void *));
	add_num = del_num = 0;
	add_buf = del_buf = 4;
	pix = NULL;

	// コンボボックスからアイテムをコピー
	CopyCBtoLB(hdlg,hcb_logo);

	// 背景色に黒を選択
	SendDlgItemMessage(hdlg,IDC_BLACK,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
	bgyc = yc_black;
}


/*--------------------------------------------------------------------
* 	on_IDOK()	OKボタン動作
*-------------------------------------------------------------------*/
static BOOL on_IDOK(HWND hdlg,WPARAM wParam)
{
	int i;

	// リストボックスからコンボボックスへコピー
	CopyLBtoCB(hdlg,hcb_logo);

	// del_listのアイテム開放
	for(i=0;i<del_num;i++)
		if(del_list[i]) free(del_list[i]);

	if(add_list) free(add_list);
	if(del_list) free(del_list);

	EndDialog(hdlg,LOWORD(wParam));
	hoptdlg = NULL;

	if(pix) VirtualFree(pix,0,MEM_RELEASE);

	return TRUE;
}

/*--------------------------------------------------------------------
* 	on_IDCANCEL()	キャンセルボタン動作
*-------------------------------------------------------------------*/
static BOOL on_IDCANCEL(HWND hdlg,WPARAM wParam)
{
	int i;

	// add_listのアイテム開放
	for(i=0;i<add_num;i++)
		if(add_list[i]) free(add_list[i]);

	if(add_list) free(add_list);
	if(del_list) free(del_list);

	EndDialog(hdlg,LOWORD(wParam));
	hoptdlg = NULL;

	if(pix) VirtualFree(pix,0,MEM_RELEASE);

	return TRUE;
}
/*--------------------------------------------------------------------
* 	on_IDC_ADD()	追加ボタン動作
*-------------------------------------------------------------------*/
static void on_IDC_ADD(HWND hdlg)
{
	char filename[MAX_PATH];
	BOOL res;

	// ロードファイル名取得
	res = optfp->exfunc->dlg_get_load_name(filename,LGD_FILTER,LGD_DEFAULT);

	if(res==FALSE)	// キャンセル
		return;

	// 読み込み
	ReadLogoData(filename,hdlg);
	DispLogo(hdlg);
}

/*--------------------------------------------------------------------
* 	on_IDC_DEL()	削除ボタン動作
*-------------------------------------------------------------------*/
static void on_IDC_DEL(HWND hdlg)
{
	int n;
	int c;

	// 選択アイテム番号取得
	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);

	if(n!=LB_ERR)
		DeleteItem(hdlg,n);	// 削除

	// アイテム数取得
	c = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCOUNT,0,0);

	// カレントセルセット
	if(c!=0){
		if(c==n) n--;
		SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,n,0);
	}
}
/*--------------------------------------------------------------------
* 	on_IDC_EXPORT()	書き出しボタン動作
*-------------------------------------------------------------------*/
static void on_IDC_EXPORT(HWND hdlg)
{
	char fname[MAX_PATH];
	void *data;
	int  n;
	BOOL res;

	// カレントセル取得
	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);

	if(n==LB_ERR){	// 選択されていない
		MessageBox(hdlg,"ロゴが選択されていません",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	data = (void *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,n,0);
	if(data==NULL){
		MessageBox(hdlg,"ロゴデータが壊れています",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	// セーブファイル名取得
	// デフォルトファイル名：ロゴ名.lgd
	wsprintf(fname,"%s.lgd",(char *)data);
	res = optfp->exfunc->dlg_get_save_name(fname,LGD_FILTER,fname);

	if(res==FALSE)	// キャンセル
		return;

	ExportLogoData(fname,data,hdlg);
}
/*--------------------------------------------------------------------
* 	on_IDC_UP()		↑ボタン動作
*-------------------------------------------------------------------*/
static void on_IDC_UP(HWND hdlg)
{
	int   n;
	void *data;
	char  str[128];

	// 選択位置取得
	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);
	if(n==0 || n==LB_ERR){	// 一番上のときor選択されていない
		SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,n,0);
		return;
	}

	// データ・文字列取得
	data = (void *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,n,0);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_GETTEXT,n,(LPARAM)str);

	// 削除
	SendDlgItemMessage(hdlg,IDC_LIST,LB_DELETESTRING,n,0);

	// 挿入
	n--;	// 一つ上
	SendDlgItemMessage(hdlg,IDC_LIST,LB_INSERTSTRING,n,(LPARAM)str);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETITEMDATA,n,(LPARAM)data);

	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,n,0);
}

/*--------------------------------------------------------------------
* 	on_IDC_DOWN()		↓ボタン動作
*-------------------------------------------------------------------*/
static void on_IDC_DOWN(HWND hdlg)
{
	int   n;
	int   count;
	void *data;
	char  str[128];

	// 選択位置取得
	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);
	count = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCOUNT,0,0);
	if(n==count-1 || n==LB_ERR){	// 一番下or選択されていない
		SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,n,0);
		return;
	}

	// データ・文字列取得
	data = (void *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,n,0);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_GETTEXT,n,(LPARAM)str);

	// 削除
	SendDlgItemMessage(hdlg,IDC_LIST,LB_DELETESTRING,n,0);

	// 挿入
	n++;	// 一つ下
	SendDlgItemMessage(hdlg,IDC_LIST,LB_INSERTSTRING,n,(LPARAM)str);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETITEMDATA,n,(LPARAM)data);

	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,n,0);
}

/*--------------------------------------------------------------------
* 	ReadLogoData()	ロゴデータを読み込む
*-------------------------------------------------------------------*/
static int ReadLogoData(char *fname,HWND hdlg)
{
	HANDLE hFile;
	LOGO_HEADER lgh;
	DWORD readed = 0;
	ULONG ptr;
	void* data;
	unsigned char num;	// ファイルに含まれるデータの数
	int n;
	int i;
	int same;
	char message[255];

	// ファイルオープン
	hFile = CreateFile(fname,GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile==INVALID_HANDLE_VALUE){
		MessageBox(hdlg,"ロゴデータファイルが見つかりません",filter_name,MB_OK|MB_ICONERROR);
		return 0;
	}
	if(GetFileSize(hFile, NULL)<=sizeof(LOGO_HEADER)){	// サイズ確認
		CloseHandle(hFile);
		MessageBox(hdlg,"ロゴデータファイルが不正です",filter_name,MB_OK|MB_ICONERROR);
		return 0;
	}

	SetFilePointer(hFile,31, 0, FILE_BEGIN);	// 先頭から31byteへ
	ReadFile(hFile,&num,1,&readed,NULL);	// データ数取得

	n = 0;	// 読み込みデータカウンタ

	for(i=0;i<num;i++){

		// LOGO_HEADER 読み込み
		readed = 0;
		ReadFile(hFile,&lgh,sizeof(LOGO_HEADER),&readed, NULL);
		if(readed!=sizeof(LOGO_HEADER)){
			MessageBox(hdlg,"ロゴデータの読み込みに失敗しました",filter_name,MB_OK|MB_ICONERROR);
			break;
		}

		// 同名ロゴがあるか
		same = SendDlgItemMessage(hdlg,IDC_LIST,LB_FINDSTRING,-1,(WPARAM)lgh.name);
		if(same!=CB_ERR){
			wsprintf(message,"同名のロゴがあります\n置き換えますか？\n\n%s",lgh.name);
			if(MessageBox(hdlg,message,filter_name,MB_YESNO|MB_ICONQUESTION)==IDYES){
				// 削除
				DeleteItem(hdlg,same);
			}
			else{	// 上書きしない
				// ファイルポインタを進める
				SetFilePointer(hFile,LOGO_PIXELSIZE(&lgh), 0, FILE_CURRENT);
				continue;
			}
		}

		// メモリ確保
		data = malloc(LOGO_DATASIZE(&lgh));
		if(data==NULL){
			MessageBox(hdlg,"メモリが足りません",filter_name,MB_OK|MB_ICONERROR);
			break;
		}

		// LOGO_HEADERコピー
		*((LOGO_HEADER *)data) = lgh;

		ptr = (unsigned long)data + sizeof(LOGO_HEADER);

		// LOGO_PIXEL読み込み
		readed = 0;
		ReadFile(hFile,(void *)ptr,LOGO_PIXELSIZE(&lgh),&readed,NULL);

		if(LOGO_PIXELSIZE(&lgh)>readed){	// 尻切れ対策
			readed -= readed % 2;
			ptr    += readed;
			memset((void *)ptr,0,LOGO_PIXELSIZE(&lgh)-readed);
		}

		// リストボックスに追加
		AddItem(hdlg,data);

		n++;
	}

	CloseHandle(hFile);

	if(n){
		// リストボックスのカーソル設定
		// 読み込んだもので一番上に
		i = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCOUNT,0,0) - n;
		SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,i,0);
	}

	return n;
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
* 	AddItem()	リストアイテムを追加
*-------------------------------------------------------------------*/
static void AddItem(HWND hdlg,void *data)
{
	int n;

	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCOUNT,0,0);

	SendDlgItemMessage(hdlg,IDC_LIST,LB_INSERTSTRING,n,(LPARAM)(char *)data);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETITEMDATA,n,(LPARAM)data);

	if(add_buf==add_num){	// バッファがいっぱいのとき
		add_buf += 4;
		add_list = realloc(add_list,add_buf*sizeof(void*));
	}

	add_list[add_num] = data;
	add_num++;
}


/*--------------------------------------------------------------------
* 	DeleteItem()	リストアイテムを削除する
*-------------------------------------------------------------------*/
static void DeleteItem(HWND hdlg,int num)
{
	if(del_buf==del_num){	// バッファがいっぱいのとき
		del_buf += 4;
		del_list = realloc(del_list,del_buf*sizeof(void *));
	}

	del_list[del_num] = (void *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,num,0);
	del_num++;

	SendDlgItemMessage(hdlg,IDC_LIST,LB_DELETESTRING,num,0);
}

/*--------------------------------------------------------------------
* 	CopyLBtoCB()	リストボックスからコンボボックスへコピー
*-------------------------------------------------------------------*/
static void CopyLBtoCB(HWND hdlg,HWND combo)
{
	int   num;
	int   i;
	void *data;
	char  str[128];

	// コンボボックスクリア
	num = SendMessage(combo,CB_GETCOUNT,0,0);
	for(i=0;i<num;i++)
		SendMessage(combo,CB_DELETESTRING,0,0);

	// コピー
	num = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCOUNT,0,0);
	for(i=0;i<num;i++){
		// アイテム取得
		data = (void *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,i,0);
		SendDlgItemMessage(hdlg,IDC_LIST,LB_GETTEXT,i,(LPARAM)str);

		// コンボボックスにセット
		SendMessage(combo,CB_INSERTSTRING,i,(LPARAM)str);
		SendMessage(combo,CB_SETITEMDATA,i,(LPARAM)data);
	}
}

/*--------------------------------------------------------------------
* 	CopyCBtoLB()	コンボボックスからリストボックスへコピー
*-------------------------------------------------------------------*/
static void CopyCBtoLB(HWND hdlg,HWND combo)
{
	int   num;
	int   i;
	void *data;
	char  str[128];

	// コピー
	num = SendMessage(combo,CB_GETCOUNT,0,0);
	for(i=0;i<num;i++){
		// アイテム取得
		data = (void *)SendMessage(combo,CB_GETITEMDATA,i,0);
		SendMessage(combo,CB_GETLBTEXT,i,(LPARAM)str);

		// リストボックスにセット
		SendDlgItemMessage(hdlg,IDC_LIST,LB_INSERTSTRING,i,(LPARAM)str);
		SendDlgItemMessage(hdlg,IDC_LIST,LB_SETITEMDATA,i,(LPARAM)data);
	}
}

/*--------------------------------------------------------------------
* 	DispLogo()	ロゴを表示
*-------------------------------------------------------------------*/
static void DispLogo(HWND hdlg)
{
	BITMAPINFO  bmi;
	LOGO_HEADER *lgh;
	LOGO_PIXEL  *lgp;
	PIXEL_YC    yc;
	PIXEL       rgb;
	int   i,j;
	HDC   hdc;
	HWND  panel;
	RECT  rec;
	int   temp;
	double magnify;	// 表示倍率

	// 選択ロゴデータ取得
	i = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);
	if(i==LB_ERR)	return;	// 選択されていない時何もしない
	lgh = (LOGO_HEADER *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,i,0);

	// BITMAPINFO設定
	ZeroMemory(&bmi,sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = lgh->w + (4-lgh->w%4);	// ４の倍数
	bmi.bmiHeader.biHeight      = lgh->h;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	// メモリ確保
	pix = VirtualAlloc(pix,bmi.bmiHeader.biWidth*bmi.bmiHeader.biHeight*sizeof(PIXEL),MEM_COMMIT,PAGE_READWRITE);
	if(pix==NULL){	// メモリ確保失敗
		return;	// 何もしない
	}

	(void *)lgp = lgh + 1;

	// RGBデータ作成
	for(i=0;i<lgh->h;i++){
		for(j=0;j<lgh->w;j++){
			// 輝度
			yc.y  = (bgyc.y*(LOGO_MAX_DP-lgp->dp_y) + lgp->y*lgp->dp_y) / LOGO_MAX_DP;
			// 色差(青)
			yc.cb = (bgyc.cb*(LOGO_MAX_DP-lgp->dp_cb) + lgp->cb*lgp->dp_cb) / LOGO_MAX_DP;
			// 色差(赤)
			yc.cr = (bgyc.cr*(LOGO_MAX_DP-lgp->dp_cr) + lgp->cr*lgp->dp_cr) / LOGO_MAX_DP;

			// YCbCr -> RGB
			optfp->exfunc->yc2rgb(&pix[(bmi.bmiHeader.biWidth)*(lgh->h-1-i)+j],&yc,1);

			lgp++;
		}
	}

	// rect設定
	panel = GetDlgItem(hdlg,IDC_PANEL);
	GetClientRect(panel,&rec);
	rec.left  = 2;
	rec.top   = 8;	// FONT-1
	rec.right  -= 3;
	rec.bottom -= 3;

	// デバイスコンテキスト取得
	hdc = GetDC(panel);

	// 塗りつぶす
	FillRect(hdc,&rec,(HBRUSH)(COLOR_ACTIVEBORDER+1));


	// 表示画像の倍率
	if(rec.right-rec.left >= lgh->w*1.5){	// 幅が収まる時
		if(rec.bottom-rec.top >= lgh->h*1.5)	// 高さも収まる時
			magnify = 1.5;
		else	// 高さのみ収まらない
			magnify = ((double)rec.bottom-rec.top)/lgh->h;
	}
	else {
		if(rec.bottom-rec.top >= lgh->h*1.5)	// 幅のみ収まらない
			magnify = ((double)rec.right-rec.left)/lgh->w;
		else{ // 幅も高さも収まらない
			magnify = ((double)rec.bottom-rec.top)/lgh->h;// 高さで計算
			magnify = (magnify>((double)rec.right-rec.left)/lgh->w) ? // 倍率が小さい方
								((double)rec.right-rec.left)/lgh->w : magnify;
		}
	}

	i = (rec.right-rec.left - lgh->w*magnify)/2 + rec.left;	// 中央に表示するように
	j = (rec.bottom-rec.top - lgh->h*magnify)/2 + rec.top;	// left,topを計算

	SetStretchBltMode(hdc,COLORONCOLOR);
	// 拡大表示
	StretchDIBits(hdc,i,j,lgh->w*magnify,lgh->h*magnify,
					0,0,lgh->w,lgh->h,pix,&bmi,DIB_RGB_COLORS,SRCCOPY);

	ReleaseDC(panel,hdc);
}

//*/
