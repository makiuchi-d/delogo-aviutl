/*********************************************************************
* 	ロゴ解析プラグイン		ver.β02
* 
* 2003
* 	04/06:	とりあえず完成。
* 	04/07:	バッファオーバーフロー回避
* 	04/09:	newとかmallocしたとたんに落ちるのはなぜ？
* 	04/10:	new[]に渡すパラメータがおかしかった為だった。修正
* 	04/14:	背景色計算関数の合計する時のバグを修正。
* 			メディアン化してから平均を取るようにした。
* 			回帰直線の取得アルゴリズムを少し変更。
* 	04/27:	ロゴ範囲最大値の変更（幅･高さに１ずつ余裕を持たせた）
* 	04/28:	解析結果ダイアログ表示中にAviUtlを終了できないように変更
* 			（エラーを出して落ちるバグ回避）
* 
* [β版公開]
* 
* 	05/10:	ロゴ範囲の最大値を約４倍にした。(logo.h)
* 			背景値計算を変更（ソートして真中らへんだけ平均）
* 			解析が255フレームを超えると落ちるバグ修正		(β02)
* 
*********************************************************************/
/*	TODO:
* 
* 	MEMO:
* 	・背景値計算改善：メディアン化してから平均とか
* 	・背景値計算改善策②：ソートして真中らへんだけで計算とか
*/
#include <windows.h>
#include <stdlib.h>
#include "..\\filter.h"
#include "..\\logo.h"
#include "scanpix.h"
#include "xywh.h"
#include "resultdlg.h"


#define Abs(x) ((x>0)? x : -x)


// ダイアログアイテム
#define ID_SCANBTN  40010
HWND scanbtn;


short dn_x,dn_y;	// マウスダウン座標
short up_x,up_y;	// アップ座標
bool  flg_mouse_down = 0;	// マウスダウンフラグ

void *logodata = NULL;	// ロゴデータ（解析結果）

//----------------------------
//	プロトタイプ宣言
//----------------------------
inline void create_dlgitem(HWND hwnd,HINSTANCE hinst);
inline void SetXYWH(FILTER* fp,void* editp);
inline void SetRange(FILTER* fp,void* editp);
void ScanLogoData(FILTER* fp,void* editp);
void SetScanPixel(FILTER* fp,ScanPixel*& sp,int,int,int,void*);
void Cal_BGcolor(PIXEL_YC& r,PIXEL_YC* pix,XYWH& xywh,int w,int h);
void CreateLogoData(FILTER* fp,ScanPixel*& sp);
int comp_short(const void* x,const void* y);
static short med_average(short* s,int n);

//----------------------------
//	FILTER_DLL構造体
//----------------------------
char filter_name[] = "ロゴ解析";
char filter_info[] = "ロゴ解析プラグイン ver.β02 by MakKi";

#define track_N 4
#if track_N
TCHAR *track_name[]   = { "位置X","位置Y","幅","高さ" };	// トラックバーの名前
int   track_default[] = { 1, 1, 1, 1 };	// トラックバーの初期値
int   track_s[]       = { 1, 1, 1, 1 };	// トラックバーの下限値
int   track_e[]       = { 1, 1, 1, 1 };	// トラックバーの上限値
#endif

#define check_N 0
#if check_N
TCHAR *check_name[]   = { 0 };	// チェックボックス
int   check_default[] = { 0 };	// デフォルト
#endif

#define tLOGOX   0
#define tLOGOY   1
#define tLOGOW   2
#define tLOGOH   3


// 設定ウィンドウの高さ
#define WND_Y (60+24*track_N+20*check_N)


FILTER_DLL filter = {
	FILTER_FLAG_WINDOW_SIZE |			// 設定ウィンドウのサイズを指定出来るようにします
	FILTER_FLAG_MAIN_MESSAGE | 	// func_WndProc()にWM_FILTER_MAIN_???のメッセージを送るようにします
	FILTER_FLAG_EX_INFORMATION,
#ifdef WND_Y
	320,WND_Y,			// 設定ウインドウのサイズ
#else
	NULL,NULL,
#endif
	filter_name,		// フィルタの名前
	track_N,        	// トラックバーの数
#if track_N
	track_name,     	// トラックバーの名前郡
	track_default,  	// トラックバーの初期値郡
	track_s,track_e,	// トラックバーの数値の下限上限
#else
	NULL,NULL,NULL,NULL,
#endif
	check_N,      	// チェックボックスの数
#if check_N
	check_name,   	// チェックボックスの名前郡
	check_default,	// チェックボックスの初期値郡
#else
	NULL,NULL,
#endif
	func_proc,   	// フィルタ処理関数
	NULL,NULL,   	// 開始時,終了時に呼ばれる関数
	NULL,        	// 設定が変更されたときに呼ばれる関数
	func_WndProc,	// 設定ウィンドウプロシージャ
	NULL,NULL,   	// システムで使用
	NULL,NULL,     	// 拡張データ領域
	filter_info,	// フィルタ情報
	NULL,			// セーブ開始直前に呼ばれる関数
	NULL,			// セーブ終了時に呼ばれる関数
	NULL,NULL,NULL,	// システムで使用
	NULL,			// 拡張領域初期値
};

/*********************************************************************
*	DLL Export
*********************************************************************/
EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable( void )
{
	return &filter;
}

/*====================================================================
*	フィルタ処理関数
*===================================================================*/
BOOL func_proc(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	// 編集中以外は何もしない
	if(!fp->exfunc->is_editing(fpip->editp))
		return FALSE;

	XYWH logo(fp->track[tLOGOX],fp->track[tLOGOY],fp->track[tLOGOW],fp->track[tLOGOH]);

	// 範囲外
	if(logo.x==0 || logo.y==0) return FALSE;
	if(logo.x+logo.w > fpip->w) return FALSE;
	if(logo.y+logo.h > fpip->h) return FALSE;

	PIXEL_YC* ptr;
	int i;

	// 枠を書き込む(Ⅰピクセル外側に）
	// X-1,Y-1に移動
	ptr = fpip->ycp_edit + (logo.x-1) + (logo.y-1) * fpip->max_w;
	// 横線（上）ネガポジ
	for(i=0;i<=logo.w+1;i++){
		ptr->y = 4096 - ptr->y;
		ptr->cb *= -1;
		ptr->cr *= -1;
		ptr++;
	}
	ptr += fpip->max_w - i;
	// 縦線
	for(i=1;i<=logo.h;i++){
		// 左線
		ptr->y = 4096 - ptr->y;
		ptr->cb *= -1;
		ptr->cr *= -1;
		// 右線
		if(logo.w>=0){
			ptr[logo.w+1].y  = 4096 - ptr[logo.w+1].y;
			ptr[logo.w+1].cb *= -1;
			ptr[logo.w+1].cr *= -1;
		}
		ptr += fpip->max_w;
	}
	// 横線（下）
	if(logo.h>=0){
		for(i=0;i<=logo.w+1;i++){
			ptr->y = 4096 - ptr->y;
			ptr->cb *= -1;
			ptr->cr *= -1;
			ptr++;
		}
	}



	return TRUE;
}

/*====================================================================
*	設定ウィンドウプロシージャ
*===================================================================*/
BOOL func_WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp )
{
	switch(message){
		case WM_FILTER_INIT:	// 初期化
			create_dlgitem(hwnd,fp->dll_hinst);
			break;

		case WM_FILTER_CHANGE_PARAM:
			SetRange(fp,editp);
			return TRUE;

		//--------------------------------------------マウスメッセージ
		case WM_FILTER_MAIN_MOUSE_DOWN:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;	// 無効の時何もしない
			dn_x = up_x = (short)LOWORD(lparam);
			dn_y = up_y = (short)HIWORD(lparam);
			flg_mouse_down = true;
			SetXYWH(fp,editp);
			return TRUE;

		case WM_FILTER_MAIN_MOUSE_UP:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;
			if(flg_mouse_down){	// マウスが押されている時
				up_x = (short)LOWORD(lparam);
				up_y = (short)HIWORD(lparam);
				flg_mouse_down = false;
				SetXYWH(fp,editp);
				return TRUE;
			}
			break;

		case WM_FILTER_MAIN_MOUSE_MOVE:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;
			if(flg_mouse_down){	// マウスが押されている時
				up_x = (short)LOWORD(lparam);
				up_y = (short)HIWORD(lparam);
				SetXYWH(fp,editp);
				return TRUE;
			}
			break;

		//----------------------------------------------ロゴ解析ボタン
		case WM_COMMAND:
			switch(LOWORD(wparam)){
				case ID_SCANBTN:
					ScanLogoData(fp,editp);
					return TRUE;
			}
			break;

		case WM_KEYUP:	// メインウィンドウへ送る
		case WM_KEYDOWN:
		case WM_MOUSEWHEEL:
			SendMessage(GetWindow(hwnd, GW_OWNER), message, wparam, lparam);
			break;
	}

	return FALSE;
}

/*--------------------------------------------------------------------
*	ダイアログアイテムを作る
*-------------------------------------------------------------------*/
inline void create_dlgitem(HWND hwnd,HINSTANCE hinst)
{
#define ITEM_Y (14+24*track_N+20*check_N)
	HFONT font;
	font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// ロゴ解析ボタン
	scanbtn = CreateWindow("BUTTON", "ロゴ解析", WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON|BS_VCENTER,
									10,ITEM_Y, 295,18, hwnd, (HMENU)ID_SCANBTN, hinst, NULL);
	SendMessage(scanbtn, WM_SETFONT, (WPARAM)font, 0);
}


/*--------------------------------------------------------------------
*	設定ウィンドウの各値を設定する
*-------------------------------------------------------------------*/
inline void SetXYWH(FILTER* fp,void* editp)
{
	int h,w;

	if(!fp->exfunc->get_frame_size(editp,&w,&h))
		// 取得失敗
		return;

	// 画像内に収める
	if(dn_x<0) dn_x = 0;
	else if(dn_x>=w) dn_x = w -1;
	if(dn_y<0) dn_y = 0;
	else if(dn_y>=h) dn_y = h -1;
	if(up_x<0) up_x = 0;
	else if(up_x>=w) up_x = w -1;
	if(up_y<0) up_y = 0;
	else if(up_y>=h) up_y = h -1;


	// 設定ウィンドの各値を設定
	fp->track_e[tLOGOX] = w;	// X最大値
	fp->track_e[tLOGOY] = h;	// Y最大値
	fp->track[tLOGOX]   = ((dn_x<up_x)?dn_x:up_x) + 1;
	fp->track[tLOGOY]   = ((dn_y<up_y)?dn_y:up_y) + 1;
	fp->track_e[tLOGOW] = w - fp->track[tLOGOX];
	fp->track_e[tLOGOH] = h - fp->track[tLOGOY];
	fp->track[tLOGOW]   = ((dn_x<up_x)?up_x:dn_x) - fp->track[tLOGOX];
	fp->track[tLOGOH]   = ((dn_y<up_y)?up_y:dn_y) - fp->track[tLOGOY];

	fp->exfunc->filter_window_update(fp);	// 更新
}

/*--------------------------------------------------------------------
*	トラックバーの最大値を設定する
*-------------------------------------------------------------------*/
inline void SetRange(FILTER* fp,void* editp)
{
	int h,w;

	if(!fp->exfunc->get_frame_size(editp,&w,&h))
		// 取得失敗
		return;

	fp->track_e[tLOGOX] = w;	// X最大値
	fp->track_e[tLOGOY] = h;	// Y最大値
	fp->track_e[tLOGOW] = w - fp->track[tLOGOX] -1;	// 幅最大値
	fp->track_e[tLOGOH] = h - fp->track[tLOGOY] -1;	// 高さ最大値

	if(fp->track_e[tLOGOX] < fp->track[tLOGOX])
		fp->track[tLOGOX] = fp->track_e[tLOGOX];	// 最大値にあわせる
	if(fp->track_e[tLOGOY] < fp->track[tLOGOY])
		fp->track[tLOGOY] = fp->track_e[tLOGOY];
	if(fp->track_e[tLOGOW] < fp->track[tLOGOW])
		fp->track[tLOGOW] = fp->track_e[tLOGOW];
	if(fp->track_e[tLOGOH] < fp->track[tLOGOH])
		fp->track[tLOGOH] = fp->track_e[tLOGOH];

	fp->exfunc->filter_window_update(fp);	// 更新
}

/*--------------------------------------------------------------------
*	ロゴデータを解析する
*-------------------------------------------------------------------*/
void ScanLogoData(FILTER* fp,void* editp)
{
	EnableWindow(scanbtn,FALSE);	// 解析ボタン無効化

	int frame_n,w,h;	// フレーム数,幅,高さ
	int frame;	// 現在の表示フレーム

	ScanPixel*  sp = NULL;
	LOGO_HEADER lgh;

	try{
		if(fp->exfunc->is_filter_active(fp)==FALSE)	// フィルタが有効でない時
			throw "フィルタを有効にしてください";

		// 必要な情報を集める
		frame   = fp->exfunc->get_frame(editp);
		frame_n = fp->exfunc->get_frame_n(editp);
		if(!frame_n) throw "映像が読み込まれていません";
		if(frame_n<2) throw "画像の枚数が足りません";

		if((fp->track[tLOGOW]+1)*(fp->track[tLOGOH]+1) > LOGO_MAXPIXEL)
			// h*wがロゴデータ上限より大きい時
			throw "ロゴ領域が広すぎます";

		// 画像サイズ
		if(!fp->exfunc->get_frame_size(editp,&w,&h))
			throw "画像サイズ取得できませんでした";

		// キャッシュサイズ設定
		fp->exfunc->set_ycp_filtering_cache_size(fp,w,h,1,NULL);

		// ロゴ名の初期値
		GetWindowText(GetWindow(fp->hwnd,GW_OWNER),defname,13);	// タイトルバー文字列取得
		for(int i=1;i<13;i++)
			if(defname[i]=='.') defname[i] = '\0';	// 最初の'.'を終端にする
		wsprintf(defname,"%s %dx%d",defname,w,h);	// デフォルトロゴ名作成

		// ScanPixelを設定する
		SetScanPixel(fp,sp,w,h,frame_n,editp);

		// 解析・ロゴデータ作成
		CreateLogoData(fp,sp);
	}
	catch(const char* str){
		MessageBox(fp->hwnd,str,filter_name,MB_OK|MB_ICONERROR);
		if(sp) delete[] sp;
		sp=NULL;
		if(logodata) delete[] logodata;
		logodata=NULL;
		EnableWindow(scanbtn,TRUE);	// ボタンを有効に戻す
		return;
	}

	if(sp){
		delete[] sp;
		sp=NULL;
	}

	SetWindowText(GetWindow(fp->hwnd,GW_OWNER),"解析完了");

	// 表示フレームを戻す
	fp->exfunc->set_frame(editp,frame);

	// 解析結果ダイアログ
	dlgfp = fp;
	DialogBox(fp->dll_hinst,"RESULT_DLG",GetWindow(fp->hwnd,GW_OWNER),ResultDlgProc);

	if(logodata){
		delete[] logodata;
		logodata=NULL;
	}

	EnableWindow(scanbtn,TRUE);	// ボタンを有効に戻す
}

/*--------------------------------------------------------------------
*	ScanPixelを設定する
*-------------------------------------------------------------------*/
void SetScanPixel(FILTER* fp,ScanPixel*& sp,int w,int h,int frame_n,void* editp)
{
	XYWH xywh(fp->track[tLOGOX],fp->track[tLOGOY],fp->track[tLOGOW],fp->track[tLOGOH]);

	// 範囲チェック
	if(xywh.w<=0 || xywh.h<=0)
		throw "領域が指定されていません";
	if( (xywh.x+xywh.w > w-1) ||
		(xywh.y+xywh.h > h-1) )
			throw "領域の一部が画面外です";

	// メモリ確保
	if(sp) delete[] sp;
	sp = new ScanPixel[xywh.w*xywh.h];	// 幅×高さの配列
	if(sp==NULL)
		throw "メモリが確保できませんでした";

	char wndtitle[128];

	for(int i=0;i<xywh.w*xywh.h;i++)
		sp[i].Alloc(frame_n);

	// 各フレーム処理
	for(int n=0;n<frame_n;n++){

		// 表示フレームを更新
		fp->exfunc->set_frame(editp,n);
		wsprintf(wndtitle,"ロゴ解析中...  [%d/%d]",n+1,frame_n);
		SetWindowText(GetWindow(fp->hwnd,GW_OWNER),wndtitle);

		// 画像取得
		PIXEL_YC* pix = fp->exfunc->get_ycp_filtering_cache_ex(fp,editp,n,NULL,NULL);

		// 背景平均値計算
		PIXEL_YC bg;
		Cal_BGcolor(bg,pix,xywh,w,h);

		// ロゴサンプルセット
		for(int i=0;i<xywh.h;i++){
			for(int j=0;j<xywh.w;j++){
				PIXEL_YC ptr;
				ptr = pix[(xywh.y+i)*w + xywh.x+j];
				sp[i*xywh.w+j].AddSample(ptr,bg);	// X軸:背景
			}
		}

	}// フレーム処理ここまで
}

/*--------------------------------------------------------------------
*	背景色計算
*-------------------------------------------------------------------*/
void Cal_BGcolor(PIXEL_YC& r,PIXEL_YC* pix,XYWH& xywh,int w,int h)
{
	short* y;	// 背景色配列
	short* cb;
	short* cr;
	int i,n;

	n = 0;

	// (幅+高さ+2)*2
	y  = new short[(xywh.w+xywh.h+2)*2];
	cb = new short[(xywh.w+xywh.h+2)*2];
	cr = new short[(xywh.w+xywh.h+2)*2];

	pix += xywh.x-1 + (xywh.y-1)*h;	// X-1,Y-1に移動

	// 横線（上）合計
	for(i=0;i<=xywh.w+1;i++){
		y[n]  = pix->y;
		cb[n] = pix->cb;
		cr[n] = pix->cr;
		n++;
		pix++;
	}
	pix += w - i;	// 次の行へ
	// 縦線
	for(i=2;i<=xywh.h+1;i++){
		// 左線
		y[n]  = pix->y;
		cb[n] = pix->cb;
		cr[n] = pix->cr;
		n++;
		// 右線
		y[n]  = pix[xywh.w+1].y;
		cb[n] = pix[xywh.w+1].cb;
		cr[n] = pix[xywh.w+1].cr;
		n++;

		pix += w;	// 次の行へ
	}
	// 横線（下）合計
	for(i=0;i<=xywh.w+1;i++){
		y[n]  = pix->y;
		cb[n] = pix->cb;
		cr[n] = pix->cr;
		n++;
		pix++;
	}

	// ソートして真中らへんを平均
	r.y  = med_average(y,n);
	r.cb = med_average(cb,n);
	r.cr = med_average(cr,n);

	delete[] y;
	delete[] cb;
	delete[] cr;
}

/*--------------------------------------------------------------------
*	ロゴデータを作成
*-------------------------------------------------------------------*/
void CreateLogoData(FILTER* fp,ScanPixel*& sp)
{
	// ロゴヘッダ作成（名称以外）
	LOGO_HEADER lgh;
	ZeroMemory(&lgh,sizeof(LOGO_HEADER));
	lgh.x = fp->track[tLOGOX];
	lgh.y = fp->track[tLOGOY];
	lgh.w = fp->track[tLOGOW];
	lgh.h = fp->track[tLOGOH];

	// ロゴデータ領域確保
	logodata = (void*) new char[LOGO_DATASIZE(&lgh)];
	if(logodata==NULL) throw "メモリ確保できませんでした";
	*((LOGO_HEADER*)logodata) = lgh;	// ヘッダコピー

	LOGO_PIXEL* lgp;
	lgp = (LOGO_PIXEL*) ((LOGO_HEADER*)logodata+1);

	for(int i=0;i<lgh.w*lgh.h;i++)
		sp[i].GetLGP(lgp[i]);
}

/*--------------------------------------------------------------------
*	short型比較関数（qsort用）
*-------------------------------------------------------------------*/
int comp_short(const void* x,const void* y)
{
	return ((int)*(const short*)x - *(const short*)y);
}

/*--------------------------------------------------------------------
*	ソートして真中らへんを平均
*-------------------------------------------------------------------*/
static short med_average(short* s,int n)
{
	double t  =0.0;
	int    nn =0;

	// ソートする
	qsort(s,n,sizeof(short),comp_short);

	// 真中らへんを平均
	for(int i=n/3;i<n-(n/3);i++,nn++)
		t += s[i];

	t = t / nn + 0.5;

	return ((short)t);
}

//*/
