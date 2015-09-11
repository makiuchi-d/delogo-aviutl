/*********************************************************************
* 	ロゴ解析プラグイン		ver 0.05
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
* 	06/16:	エラーメッセージを一部修正
* 			ロゴ範囲の形に色が変になるバグ修正
* 			wとhを間違えていたなんて…鬱だ
* 			背景が単一色かどうかを内部で判定するようにした
* 			選択範囲内だけを解析するようにした
* 	06/17:	昨日の修正で入れてしまったバグを修正	(β03)
* 	06/18:	最初のフレームを表示させていると解析できないバグ修正 (β03a)
* 			結果ダイアログのプレビュー背景色をRGBで指定するように変更
* 
* [正式版]
* 	07/02:	中断できるようにした。
* 			このために処理の流れを大幅に変更。
* 	07/03:	プロファイルの変更フレームで無限ループになるバグ回避（0.04)
* 	08/02:	処理順序の見直しして高速化
* 			細かな修正、解析完了時にビープを鳴らすようにした。
* 	09/22:	キャッシュの幅と高さを８の倍数にした。(SSE2対策になったかな?)
* 	09/27:	filter.hをAviUtl0.99SDKのものに差し替え。(0.05)
* 
*********************************************************************/
/*	TODO:
* 	・拡大ツール機能（気まぐれバロンさんのアイディア)
* 
* 	MEMO:
* 	・背景値計算改善策①：メディアン化してから平均とか
* 	・背景値計算改善策②：ソートして真中らへんだけで計算とか
* 
* 	・背景が単色かどうかの判定：背景値の平均と、最大or最小との差が閾値以上のとき単一でないとするのはどうか
* 		→最大と最小の差が閾値以上のとき単一でないと判断のほうがよさそう。
* 
* 	・SSE2処理時に落ちる：get_ycp_filtering_cache_exがぁゃιぃ。とりあえず幅高さを８の倍数に。
* 
*/
#include <windows.h>
#include <stdlib.h>
#include "..\\filter.h"
#include "..\\logo.h"
#include "scanpix.h"
#include "resultdlg.h"
#include "abort.h"


// ボタン
#define ID_SCANBTN  40010
HWND scanbtn;

static short dn_x,dn_y;	// マウスダウン座標
static short up_x,up_y;	// アップ座標
static bool  flg_mouse_down = 0;	// マウスダウンフラグ
static short _x,_y,_w,_h,_thy;

void *logodata = NULL;	// ロゴデータ（解析結果）

//----------------------------
//	プロトタイプ宣言
//----------------------------
inline void create_dlgitem(HWND hwnd,HINSTANCE hinst);
inline void SetXYWH(FILTER* fp,void* editp);
inline void SetRange(FILTER* fp,void* editp);
inline void FixXYWH(FILTER* fp,void* editp);
void ScanLogoData(FILTER* fp,void* editp);
void SetScanPixel(FILTER*,ScanPixel*&,int,int,int,int,void*);

//----------------------------
//	FILTER_DLL構造体
//----------------------------
char filter_name[] = "ロゴ解析";
char filter_info[] = "ロゴ解析プラグイン ver 0.05 by MakKi";

#define track_N 5
#if track_N
TCHAR *track_name[]   = { "位置X","位置Y","幅","高さ","閾値" };	// トラックバーの名前
int   track_default[] = { 1, 1, 1, 1,  30 };	// トラックバーの初期値
int   track_s[]       = { 1, 1, 1, 1,   0 };	// トラックバーの下限値
int   track_e[]       = { 1, 1, 1, 1, 255 };	// トラックバーの上限値
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
#define tTHY     4

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

	// 範囲外
	if(fp->track[tLOGOX]==0 || fp->track[tLOGOY]==0) return FALSE;
	if(fp->track[tLOGOX]+fp->track[tLOGOW] > fpip->w) return FALSE;
	if(fp->track[tLOGOY]+fp->track[tLOGOH] > fpip->h) return FALSE;

	PIXEL_YC* ptr;
	int i;

	// 枠を書き込む(Ⅰピクセル外側に）
	// X-1,Y-1に移動
	ptr = fpip->ycp_edit + (fp->track[tLOGOX]-1) + (fp->track[tLOGOY]-1) * fpip->max_w;
	// 横線（上）ネガポジ
	for(i=0;i<=fp->track[tLOGOW]+1;i++){
		ptr->y = 4096 - ptr->y;
		ptr->cb *= -1;
		ptr->cr *= -1;
		ptr++;
	}
	ptr += fpip->max_w - i;
	// 縦線
	for(i=1;i<=fp->track[tLOGOH];i++){
		// 左線
		ptr->y = 4096 - ptr->y;
		ptr->cb *= -1;
		ptr->cr *= -1;
		// 右線
		if(fp->track[tLOGOW]>=0){
			ptr[fp->track[tLOGOW]+1].y  = 4096 - ptr[fp->track[tLOGOW]+1].y;
			ptr[fp->track[tLOGOW]+1].cb *= -1;
			ptr[fp->track[tLOGOW]+1].cr *= -1;
		}
		ptr += fpip->max_w;
	}
	// 横線（下）
	if(fp->track[tLOGOH]>=0){
		for(i=0;i<=fp->track[tLOGOW]+1;i++){
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
	static bool scanning;

	switch(message){
		case WM_FILTER_INIT:	// 初期化
			create_dlgitem(hwnd,fp->dll_hinst);
			scanning = false;
			break;

		case WM_FILTER_CHANGE_PARAM:
			if(scanning){
				FixXYWH(fp,editp);
				return TRUE;
			}
			else
				SetRange(fp,editp);
			return FALSE;

		//--------------------------------------------マウスメッセージ
		case WM_FILTER_MAIN_MOUSE_DOWN:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;	// 無効の時何もしない
			dn_x = up_x = (short)LOWORD(lparam);
			dn_y = up_y = (short)HIWORD(lparam);
			flg_mouse_down = true;
			if(!scanning) SetXYWH(fp,editp);
			return TRUE;

		case WM_FILTER_MAIN_MOUSE_UP:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;
			if(flg_mouse_down){	// マウスが押されている時
				up_x = (short)LOWORD(lparam);
				up_y = (short)HIWORD(lparam);
				flg_mouse_down = false;
				if(!scanning) SetXYWH(fp,editp);
				return TRUE;
			}
			break;

		case WM_FILTER_MAIN_MOUSE_MOVE:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;
			if(flg_mouse_down){	// マウスが押されている時
				up_x = (short)LOWORD(lparam);
				up_y = (short)HIWORD(lparam);
				if(!scanning) SetXYWH(fp,editp);
				return TRUE;
			}
			break;

		//----------------------------------------------ロゴ解析ボタン
		case WM_COMMAND:
			switch(LOWORD(wparam)){
				case ID_SCANBTN:
					scanning = true;
					ScanLogoData(fp,editp);
					scanning = false;
					break;
			}
			break;

		case WM_KEYUP:	// メインウィンドウへ送る
		case WM_KEYDOWN:
		case WM_MOUSEWHEEL:
			SendMessage(GetWindow(hwnd, GW_OWNER), message, wparam, lparam);
			break;

		//----------------------------------------------独自メッセージ
		case WM_SP_DRAWFRAME:
			fp->exfunc->set_frame(editp,lparam);
			return TRUE;
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
*	設定ウィンドウの各値を固定する
*-------------------------------------------------------------------*/
inline void FixXYWH(FILTER* fp,void* editp)
{
	fp->track[tLOGOX] = _x;
	fp->track[tLOGOY] = _y;
	fp->track[tLOGOW] = _w;
	fp->track[tLOGOH] = _h;
	fp->track[tTHY]   = _thy;

	fp->exfunc->filter_window_update(fp);	// 更新
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

	_x = fp->track[tLOGOX]; _y = fp->track[tLOGOY];
	_w = fp->track[tLOGOW]; _h = fp->track[tLOGOH];
	_thy = fp->track[tTHY];

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

	_x = fp->track[tLOGOX]; _y = fp->track[tLOGOY];
	_w = fp->track[tLOGOW]; _h = fp->track[tLOGOH];
	_thy = fp->track[tTHY];

	fp->exfunc->filter_window_update(fp);	// 更新
}

/*--------------------------------------------------------------------
*	ロゴデータを解析する
*-------------------------------------------------------------------*/
void ScanLogoData(FILTER* fp,void* editp)
{
	EnableWindow(scanbtn,FALSE);	// 解析ボタン無効化

	int w,h;		// 幅,高さ
	int start,end;	// 選択開始・終了フレーム
	int frame;		// 現在の表示フレーム

	ScanPixel*  sp = NULL;
	LOGO_HEADER lgh;

	try{
		if(fp->exfunc->is_filter_active(fp)==FALSE)	// フィルタが有効でない時
			throw "フィルタを有効にしてください";

		// 必要な情報を集める
		frame   = fp->exfunc->get_frame_n(editp);
		if(!frame) throw "映像が読み込まれていません";

		frame   = fp->exfunc->get_frame(editp);

		fp->exfunc->get_select_frame(editp,&start,&end);
		if(end-start<1) throw "画像の枚数が足りません";

		if((fp->track[tLOGOW]+1)*(fp->track[tLOGOH]+1) > LOGO_MAXPIXEL)
			// h*wがロゴデータ上限より大きい時
			throw "ロゴ領域が広すぎます";

		// 画像サイズ
		if(!fp->exfunc->get_frame_size(editp,&w,&h))
			throw "画像サイズ取得できませんでした";

		// ロゴ名の初期値
		GetWindowText(GetWindow(fp->hwnd,GW_OWNER),defname,LOGO_MAX_NAME-9);	// タイトルバー文字列取得
		for(int i=1;i<LOGO_MAX_NAME-9;i++)
			if(defname[i]=='.') defname[i] = '\0';	// 2文字目以降の'.'を終端にする（.aviを削除）
		wsprintf(defname,"%s %dx%d",defname,w,h);	// デフォルトロゴ名作成

		// キャッシュサイズ設定
		w += w % 8;
		h += h % 8;
		fp->exfunc->set_ycp_filtering_cache_size(fp,w,h,1,NULL);

		// ScanPixelを設定する+解析・ロゴデータ作成
		SetScanPixel(fp,sp,w,h,start,end,editp);
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
void SetScanPixel(FILTER* fp,ScanPixel*& sp,int w,int h,int s,int e,void* editp)
{
	// 範囲チェック
	if(fp->track[tLOGOW]<=0 || fp->track[tLOGOH]<=0)
		throw "領域が指定されていません";
	if( (fp->track[tLOGOX]+fp->track[tLOGOW] > w-1) ||
		(fp->track[tLOGOY]+fp->track[tLOGOH] > h-1) )
			throw "領域の一部が画面外です";

	// メモリ確保
	if(sp) delete[] sp;
	ScanPixel::Defbuf = 1024;
	sp = new ScanPixel[fp->track[tLOGOW]*fp->track[tLOGOH]];	// 幅×高さの配列
	if(sp==NULL)
		throw "メモリが確保できませんでした";

	AbortDlgParam param;

	param.fp     = fp;
	param.editp  = editp;
	param.sp     = sp;
	param.s      = s;
	param.e      = e;
	param.max_w  = w;
	param.x      = fp->track[tLOGOX];
	param.y      = fp->track[tLOGOY];
	param.w      = fp->track[tLOGOW];
	param.h      = fp->track[tLOGOH];
	param.t      = fp->track[tTHY];
	param.data   = &logodata;
	param.errstr = NULL;

	DialogBoxParam(fp->dll_hinst,"ABORT_DLG",GetWindow(fp->hwnd,GW_OWNER),AbortDlgProc,(LPARAM)&param);

	if(param.errstr)
		throw param.errstr;
}



//*/
