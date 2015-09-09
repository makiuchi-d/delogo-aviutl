/*********************************************************************
* 	透過性ロゴ（BSマークとか）除去フィルタ
* 								ver 0.02
* 
* 2003
* 	02/01:	製作開始
* 	02/02:	拡張データ領域を使うとプロファイル切り替えた時AviUtlがくたばる。
* 	      	なぜ？バグ？どうしよう。
* 	      	と思ったら、領域サイズに制限があるのね… SDKには一言も書いてないけど。
* 	      	消えた－－!!（Ｂだけ）ちょっと感動。
* 	      	BSロゴって輝度だけの変化なのかぁ。RGBでやると色変わるわけだ。
* 	02/06:	プロファイルの途中切り替えに仮対応。
* 	02/08:	BS2ロゴ実装。（テスト用ver.1）
* 	02/11:	YCbCrを微調整できるようにした。
* 	02/17:	試験的に外部ファイル読み込み機能を搭載。（５つまで）
* 	03/02:	ロゴデータ保存領域関係（ポインタをコンボアイテムに関連付け）
* 	03/24:	ロゴデータ読み込み関係
* 	03/25:	フィルタ本体とロゴデータを切り離した。
* 	03/26:	ロゴデータの管理を完全にコンボアイテムと関連付け
* 	03/29:	ロゴデータ入出力ダイアログ（オプションダイアログ）作成
* 	03/30:	ロゴ付加モード追加
* 	04/03:	ロゴデータの構造を変更（dpをdp_y,dp_cb,dp_crに分離）
* 			オプションダイアログにプレビュー機能を追加
* 	04/06:	ロゴ解析アルゴリズムと、それを用いたフィルタのテスト版が
* 			完成したため計算式をそれに最適化。（不要な分岐を減らした）
* 	04/07:	ロゴプレビューの背景色を変更できるようにした。
* 	04/09:	解析プラグインからのデータを受信出来るようにした。
* 			深度調整の方法を変更(ofset->gain)
* 			プレビューで枠からはみ出さないようにした。
* 	04/20:	フィルタ名変更。ロゴ付加モードを一時廃止。
* 	04/28:	1/4単位位置調整実装。
* 			ロゴ付加モード(あまり意味ないけど)復活
* 			オプションダイアログ表示中にAviUtlを終了できないように変更
* 			（エラーを出して落ちるバグ回避）
* 
* [正式版(0.01)公開]
* 
* 	05/04:	不透明度調整の方法を変更。
* 	05/08:	メモリ関連ルーチン変更		(0.02)
* 				VFAPI動作に対応、プロファイルの途中切り替えに対応
* 				ロゴデータのサイズを約４倍にした。
* 
*********************************************************************/

/* ToDo:
* 	・BS1/2他、いろんなロゴの実装
* 	・ロゴデータの作成・編集機能
* 
*  MEMO:
* 	・ロゴの拡大縮小ルーチン自装しないとだめかなぁ。
* 		→必要なさげ。当面は自装しない。
* 	・ロゴ作成・編集は別アプリにしてしまおうか…
* 	・ロゴ除去モードとロゴ付加モードを切り替えられるようにしようかな
* 		→付けてみた
* 	・解析プラグからデータを受け取るには…独自WndMsg登録してSendMessageで送ってもらう
* 		→ちゃんと動いた。
* 	・ロゴに１ピクセル未満のズレがある。1/4ピクセルでの位置調整が必要そう。
* 		→実装完了
* 	・ダイアログを表示したまま終了するとエラー吐く
* 		→親ウィンドウをAviUtl本体にすることで終了できなくした
* 	・ロゴデータ構造少し変えようかな… 色差要素のビットを半分にするとか。ver0.10で出すか？
* 
*  新メモリ管理について:(2003/05/08)
* 	fp->ex_data_ptrにはロゴの名称のみを保存。（7FFDバイトしかプロファイルに保存されず、不具合が生じるため）
* 	func_init()でロゴデータパックを読み込む。ldpファイル名は必ずフルパスであることが必要。
* 	読み込んだロゴデータのポインタはlogodata配列に保存。配列のデータ数はlogodata_nに。
* 	func_proc()ではex_data（ロゴ名称）と一致するデータをlogodata配列から検索。なかった場合は何もしない。
* 	位置パラメータを使って位置調整データを作成。その後で除去・付加関数を呼ぶ。
* 	WndProcでは、WM_FILTER_INITでコンボボックスアイテムをlogodata配列から作る。
* 	ITEMDATAには従来どおりロゴデータのポインタを保存する。
* 	WM_FILTER_INITではコンボボックスアイテムからファイルに保存。（今までどおり）
* 	オプション設定ダイアログでのロゴデータの読み込み・削除は今までどおり。
* 	OKボタンが押されたときは、リストアイテムからlogodata配列を作り直す。コンボアイテムの更新は今までどおり。
* 
*/
#include <windows.h>
#include <commctrl.h>
#include <math.h>
#include "filter.h"
#include "logo.h"
#include "optdlg.h"
#include "resource.h"
#include "send_lgd.h"


#define ID_BUTTON_OPTION 40001
#define ID_COMBO_LOGO    40002

#define Abs(x) ((x>0)? x:-x)


#define LDP_KEY     "logofile"
#define LDP_DEFAULT "logodata.ldp"
#define LDP_FILTER  "ロゴデータパック (*.ldp)\0*.ldp\0"\
                    "AllFiles (*.*)\0*.*\0"


// ダイアログアイテム
typedef struct {
	HFONT font;
	HWND  cb_logo;
	HWND  bt_opt;
} FILTER_DIALOG;

FILTER_DIALOG dialog;

char  logodata_file[MAX_PATH] = { 0 };	// ロゴデータファイル名(INIに保存)
UINT  WM_SEND_LOGO_DATA;

LOGO_HEADER** logodata   = NULL;
unsigned int  logodata_n = 0;

char ex_data[LOGO_MAX_NAME];	// 拡張データ領域

//----------------------------
//	プロトタイプ宣言
//----------------------------
static void on_wm_filter_init(FILTER* fp);
static void on_wm_filter_exit(FILTER* fp);
static void init_dialog(HWND hwnd,HINSTANCE hinst);
static void update_cb_logo(char *name);
static void change_param(void);
static void set_cb_logo(FILTER* fp);
static int  set_combo_item(void* data);
static void del_combo_item(int num);
static void read_logo_pack(char *logodata_file,FILTER *fp);
static void set_sended_data(void* logodata,FILTER* fp);
static BOOL create_adj_exdata(FILTER *fp,LOGO_HEADER *adjdata,const LOGO_HEADER *data);
static int  find_logo(const char *logo_name);

static BOOL on_option_button(FILTER* fp);

BOOL func_proc_eraze_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh);
BOOL func_proc_add_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh);

//----------------------------
//	FILTER_DLL構造体
//----------------------------
char filter_name[] = LOGO_FILTER_NAME;
char filter_info[] = LOGO_FILTER_NAME" ver 0.02 by MakKi";
#define track_N 6
#if track_N
TCHAR *track_name[]   = { "位置 X", "位置 Y", "深度", "Y", "Cb", "Cr" };	// トラックバーの名前
int   track_default[] = {   0,   0,   100,   0,   0,   0 };	// トラックバーの初期値
int   track_s[]       = { -200, -200,   0, -100, -100, -100 };	// トラックバーの下限値
int   track_e[]       = {  200,  200, 200,  100,  100,  100 };	// トラックバーの上限値
#endif
#define check_N 2
#if check_N
TCHAR *check_name[]   = { "ロゴ付加モード","ロゴ除去モード" };	// チェックボックス
int   check_default[] = { 0, 1 };	// デフォルト
#endif


#define LOGO_X      0
#define LOGO_Y      1
#define LOGO_YDP    2
#define LOGO_CBDP   2
#define LOGO_CRDP   2
#define LOGO_PY     3
#define LOGO_CB     4
#define LOGO_CR     5

// 設定ウィンドウの高さ
#define WND_Y (67+24*track_N+20*check_N)


FILTER_DLL filter = {
	FILTER_FLAG_WINDOW_SIZE |	//	フィルタのフラグ
	FILTER_FLAG_RADIO_BUTTON |
	FILTER_FLAG_EX_DATA |
	FILTER_FLAG_EX_INFORMATION,
	320,WND_Y,			// 設定ウインドウのサイズ
	filter_name,    	// フィルタの名前
#if track_N
	track_N,        	// トラックバーの数
	track_name,     	// トラックバーの名前郡
	track_default,  	// トラックバーの初期値郡
	track_s,track_e,	// トラックバーの数値の下限上限
#else
	0,NULL,NULL,NULL,NULL,
#endif
#if check_N
	check_N,      	// チェックボックスの数
	check_name,   	// チェックボックスの名前郡
	check_default,	// チェックボックスの初期値郡
#else
	0,NULL,NULL,
#endif
	func_proc,   	// フィルタ処理関数
	func_init,		// 開始時に呼ばれる
	func_exit,   	// 終了時に呼ばれる関数
	NULL,        	// 設定が変更されたときに呼ばれる関数
	func_WndProc,	// 設定ウィンドウプロシージャ
	NULL,NULL,   	// システムで使用
	ex_data,     	// 拡張データ領域
	sizeof(LOGO_HEADER),//57102,	// 拡張データサイズ
	filter_info, 	// フィルタ情報
	NULL,			// セーブ開始直前に呼ばれる関数
	NULL,			// セーブ終了時に呼ばれる関数
	NULL,NULL,NULL,	// システムで使用
	ex_data,		// 拡張領域初期値
};




/*********************************************************************
*	DLL Export
*********************************************************************/
EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable( void )
{
	return &filter;
}

/*====================================================================
*	開始時に呼ばれる関数
*===================================================================*/
BOOL func_init( FILTER *fp )
{
	ULONG   ptr;
	HANDLE  hFile;

	// INIからロゴデータファイル名を読み込む
	fp->exfunc->ini_load_str(fp,LDP_KEY,logodata_file,NULL);

	if(lstrlen(logodata_file)==0){	// ロゴデータファイル名がなかったとき
		// 読み込みダイアログ
		if(!fp->exfunc->dlg_get_load_name(logodata_file,LDP_FILTER,LDP_DEFAULT)){
			// キャンセルされた
			MessageBox(fp->hwnd,"ロゴデータファイルがありません",filter_name,MB_OK|MB_ICONWARNING);
			return FALSE;
		}
	}
	else{	// ロゴデータファイル名があるとき
		// 存在を調べる
		hFile = CreateFile(logodata_file, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

		if(hFile==INVALID_HANDLE_VALUE){	// みつからなかったとき
			MessageBox(fp->hwnd,"ロゴデータファイルが見つかりません",filter_name,MB_OK|MB_ICONWARNING);
			if(!fp->exfunc->dlg_get_load_name(logodata_file,LDP_FILTER,LDP_DEFAULT)){
				// キャンセルされた
				MessageBox(fp->hwnd,"ロゴデータファイルが指定されていません",filter_name,MB_OK|MB_ICONWARNING);
				return FALSE;
			}
		}
		else
			CloseHandle(hFile);
	}

	// ロゴファイル読み込み
	read_logo_pack(logodata_file,fp);

	if(logodata_n)
		// 拡張データ初期値設定
		fp->ex_data_def = logodata[0];

	return TRUE;
}

/*====================================================================
*	終了時に呼ばれる関数
*===================================================================*/
BOOL func_exit( FILTER *fp )
{
	unsigned int i;

	// ロゴデータ開放
	if(logodata){
		for(i=0;i<logodata_n;i++){
			if(logodata[i])
				free(logodata[i]);
		}
		free(logodata);
	}

	return TRUE;
}

/*====================================================================
*	フィルタ処理関数
*===================================================================*/
BOOL func_proc(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	int num;
	char adjdata[LOGO_MAXSIZE];

	num = find_logo(fp->ex_data_ptr);
	if(num<0) return FALSE;

	if(!create_adj_exdata(fp,(void *)adjdata,logodata[num]))
		return FALSE;

	if(fp->check[1])	// 除去モードチェック
		return func_proc_eraze_logo(fp,fpip,(void *)adjdata);	// ロゴ除去モード
	else
		return func_proc_add_logo(fp,fpip,(void *)adjdata);		// ロゴ付加モード
}

/*--------------------------------------------------------------------
* 	func_proc_eraze_logo()	ロゴ除去モード
*-------------------------------------------------------------------*/
BOOL func_proc_eraze_logo(FILTER* fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh)
{
	int   i,j;
	int   y,cb,cr;
	PIXEL_YC    *ptr;
	LOGO_PIXEL  *lgp;
	double dp,temp;


	// LOGO_PIXELデータへのポインタ
	(void *)lgp = lgh + 1;

	// 左上の位置へ移動
	ptr = fpip->ycp_edit;
	ptr += lgh->x + lgh->y * fpip->max_w;

	for(i=0;i < lgh->h;i++){
		for(j=0;j < lgh->w;j++){

			if(ptr >= fpip->ycp_edit &&	// 画面内の時のみ処理
			   ptr < fpip->ycp_edit + fpip->max_w*fpip->h)
			{
				// 輝度
				dp = lgp->dp_y * ((double)fp->track[LOGO_YDP]/100);	// 調整
				y  = lgp->y + fp->track[LOGO_PY]*16;
				if(dp==LOGO_MAX_DP) dp--;	// 0での除算回避
				temp = ((double)ptr->y*LOGO_MAX_DP - y*dp) / (LOGO_MAX_DP - dp) +0.5;	// 逆算
				if  (temp>4096) temp = 4096;	// 範囲チェック
				else if(temp<0) temp = 0;
				ptr->y = temp;

				// 色差(青)
				dp = lgp->dp_cb * ((double)fp->track[LOGO_CBDP]/100);
				cb = lgp->cb    + fp->track[LOGO_CB]*16;
				if(dp==LOGO_MAX_DP) dp--;	// 0での除算回避
				temp = ((double)ptr->cb*LOGO_MAX_DP - cb*dp) / (LOGO_MAX_DP - dp) +0.5;
				if      (temp>2048) temp =  2048;	// 範囲チェック
				else if(temp<-2048) temp = -2048;
				ptr->cb = temp;

				// 色差(赤)
				dp = lgp->dp_cr * ((double)fp->track[LOGO_CRDP]/100);
				cr = lgp->cr   + fp->track[LOGO_CR]*16;
				if(dp==LOGO_MAX_DP) dp--;	// 0での除算回避
				temp = ((double)ptr->cr*LOGO_MAX_DP - cr*dp) / (LOGO_MAX_DP - dp) +0.5;
				if      (temp>2048) temp =  2048;	// 範囲チェック
				else if(temp<-2048) temp = -2048;
				ptr->cr = temp;

			}	// if画面内

			ptr++;	// 隣りへ
			lgp++;
		}
		// 次のラインへ
		ptr += fpip->max_w - j;
	}

	return TRUE;
}

/*--------------------------------------------------------------------
* 	func_proc_add_logo()	ロゴ付加モード
*-------------------------------------------------------------------*/
BOOL func_proc_add_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh)
{
	int   i,j;
	int   y,cb,cr;
	PIXEL_YC    *ptr;
	LOGO_PIXEL  *lgp;
	double dp,temp;


	// LOGO_PIXELデータへのポインタ
	(void *)lgp = lgh + 1;

	// 左上の位置へ移動
	ptr = fpip->ycp_edit;
	ptr += lgh->x + lgh->y * fpip->max_w;

	for(i=0;i < lgh->h;i++){
		for(j=0;j < lgh->w;j++){

			if(ptr >= fpip->ycp_edit &&	// 画面内の時のみ処理
			   ptr < fpip->ycp_edit + fpip->max_w*fpip->h)
			{

				// 輝度
				dp = lgp->dp_y * ((double)fp->track[LOGO_YDP]/100);
				y  = lgp->y    + fp->track[LOGO_PY]*16;
				temp  = ((double)ptr->y*(LOGO_MAX_DP-dp) + y*dp) / LOGO_MAX_DP +0.5;	// ロゴ付加
				if  (temp>4096) temp = 4096;	// 範囲チェック
				else if(temp<0) temp = 0;
				ptr->y = temp;


				// 色差(青)
				dp = lgp->dp_cb * ((double)fp->track[LOGO_CBDP]/100);
				cb = lgp->cb    + fp->track[LOGO_CB]*16;
				temp  = ((double)ptr->cb*(LOGO_MAX_DP-dp) + cb*dp) / LOGO_MAX_DP +0.5;
				if     (temp> 2048) temp =  2048;	// 範囲チェック
				else if(temp<-2048) temp = -2048;
				ptr->cb = temp;

				// 色差(赤)			//pow(2,(double)fp->track[LOGO_CRDP]/128);
				dp = lgp->dp_cr * ((double)fp->track[LOGO_CRDP]/100);
				cr = lgp->cr   + fp->track[LOGO_CR]*16;
				temp  = ((double)ptr->cr*(LOGO_MAX_DP-dp) + cr*dp) / LOGO_MAX_DP +0.5;
				if     (temp> 2048) temp =  2048;	// 範囲チェック
				else if(temp<-2048) temp = -2048;
				ptr->cr = temp;

			}	// if画面内

			ptr++;	// 隣りへ
			lgp++;
		}
		// 次のラインへ
		ptr += fpip->max_w - j;
	}

	return TRUE;
}

/*--------------------------------------------------------------------
* 	find_logo()		ロゴ名からロゴデータを検索
*-------------------------------------------------------------------*/
static int  find_logo(const char *logo_name)
{
	unsigned int i;

	for(i=0;i<logodata_n;i++){
		if(lstrcmp((char *)logodata[i],logo_name)==0)
			return i;
	}

	return -1;
}

/*====================================================================
*	設定ウィンドウプロシージャ
*===================================================================*/
BOOL func_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void *editp, FILTER *fp )
{
	static UINT  WM_SEND_LOGO_DATA;

	if(message==WM_SEND_LOGO_DATA){	// ロゴデータ受信
		set_sended_data((void *)wParam,fp);
		return TRUE;
	}

	switch(message){
		case WM_FILTER_INIT:	// 初期化
			on_wm_filter_init(fp);
			return TRUE;

		case WM_FILTER_EXIT:	// 終了
			on_wm_filter_exit(fp);
			break;

		case WM_FILTER_UPDATE:	// フィルタ更新
		case WM_FILTER_SAVE_END:	// セーブ終了
			// コンボボックス表示更新
			update_cb_logo(ex_data);
			break;

		//---------------------------------------------ボタン動作
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case ID_BUTTON_OPTION:	// オプションボタン
					return on_option_button(fp);

				case ID_COMBO_LOGO:	// コンボボックス
					switch(HIWORD(wParam)){
						case CBN_SELCHANGE:	// 選択変更
							change_param();
							return TRUE;
					}
			}
			break;

		case WM_KEYUP:	// メインウィンドウへ送る
		case WM_KEYDOWN:
		case WM_MOUSEWHEEL:
			SendMessage(GetWindow(hwnd, GW_OWNER), message, wParam, lParam);
			break;
	}

	return FALSE;
}

/*--------------------------------------------------------------------
* 	on_wm_filter_init()		設定ウィンドウ初期化
*-------------------------------------------------------------------*/
static void on_wm_filter_init(FILTER* fp)
{
	unsigned int i;

	init_dialog(fp->hwnd,fp->dll_hinst);
	// コンボアイテムセット
	for(i=0;i<logodata_n;i++)
		set_combo_item(logodata[i]);

	// ロゴデータ受信メッセージ登録
	WM_SEND_LOGO_DATA = RegisterWindowMessage(wm_send_logo_data);
}

/*--------------------------------------------------------------------
* 	on_wm_filter_exit()		終了処理
* 		読み込まれているロゴデータをldpに保存
*-------------------------------------------------------------------*/
static void on_wm_filter_exit(FILTER* fp)
{
	int    i,num;
	char   n;
	DWORD  dw;
	HANDLE hFile;
	void*  data;

	if(lstrlen(logodata_file)==0){	// ロゴデータファイル名がないとき
		if(!fp->exfunc->dlg_get_load_name(logodata_file,LDP_FILTER,LDP_DEFAULT)){
			// キャンセルされた
			MessageBox(fp->hwnd,"ロゴデータは保存されません",filter_name,MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else{	// ロゴデータファイル名があるとき
		// 存在を調べる
		hFile = CreateFile(logodata_file, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

		if(hFile==INVALID_HANDLE_VALUE){	// みつからなかったとき
			MessageBox(fp->hwnd,"ロゴデータファイルが見つかりません",filter_name,MB_OK|MB_ICONWARNING);
			if(!fp->exfunc->dlg_get_load_name(logodata_file,LDP_FILTER,LDP_DEFAULT)){
				// キャンセルされた
				MessageBox(fp->hwnd,"ロゴデータは保存されません",filter_name,MB_OK|MB_ICONWARNING);
				return;
			}
		}
		else
			CloseHandle(hFile);
	}

	// 登録されているアイテムの数
	num = SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0);

	// ファイルオープン
	hFile = CreateFile(logodata_file,GENERIC_WRITE, 0, 0,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	SetFilePointer(hFile,0, 0, FILE_BEGIN);	// 先頭へ

	// ヘッダ書き込み
	dw = 0;
	WriteFile(hFile,LOGO_FILE_HEADER,32,&dw,NULL);
	if(dw!=32){	// 書き込み失敗
		MessageBox(fp->hwnd,"ロゴデータ保存に失敗しました(1)",filter_name,MB_OK|MB_ICONERROR);
	}
	else{	// 成功
		n = 0;
		// 各データ書き込み
		for(i=0;i<num;i++){
			dw = 0;
			data = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,i,0);	// データのポインタ取得
			WriteFile(hFile,data,LOGO_DATASIZE(data),&dw,NULL);
			if(dw != LOGO_DATASIZE(data)){
				MessageBox(fp->hwnd,"ロゴデータ保存に失敗しました(2)",filter_name,MB_OK|MB_ICONERROR);
				break;
			}
			n++;
		}

		SetFilePointer(hFile,31, 0, FILE_BEGIN);	// 先頭から31byteへ
		dw = 0;
		WriteFile(hFile,&n,1,&dw,NULL);
		if(dw!=1)
			MessageBox(fp->hwnd,"ロゴデータ保存に失敗しました(3)",filter_name,MB_OK|MB_ICONERROR);
	}

	CloseHandle(hFile);

	// INIにロゴデータファイル名保存
	fp->exfunc->ini_save_str(fp,LDP_KEY,logodata_file);
}

/*--------------------------------------------------------------------
*	init_dialog()		ダイアログアイテムを作る
*		・コンボボックス
*		・オプションボタン
*-------------------------------------------------------------------*/
static void init_dialog(HWND hwnd,HINSTANCE hinst)
{
#define ITEM_Y (19+24*track_N+20*check_N)

	// フォント作成
	dialog.font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// コンボボックス
	dialog.cb_logo = CreateWindow("COMBOBOX", "", WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL,
									60,ITEM_Y, 175,100, hwnd, (HMENU)ID_COMBO_LOGO, hinst, NULL);
	SendMessage(dialog.cb_logo, WM_SETFONT, (WPARAM)dialog.font, 0);

	// オプションボタン
	dialog.bt_opt = CreateWindow("BUTTON", "オプション", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_VCENTER,
									240,ITEM_Y, 63,20, hwnd, (HMENU)ID_BUTTON_OPTION, hinst, NULL);
	SendMessage(dialog.bt_opt, WM_SETFONT, (WPARAM)dialog.font, 0);
}

/*--------------------------------------------------------------------
*	create_adj_exdata()		位置調整ロゴデータ作成
*-------------------------------------------------------------------*/
static BOOL create_adj_exdata(FILTER *fp,LOGO_HEADER *adjdata,const LOGO_HEADER *data)
{
	int  i,j;
	int  w,h;
	int  temp;
	int  adjx,adjy;
	LOGO_PIXEL *df;
	LOGO_PIXEL *ex;

	if(data == NULL)
		return FALSE;

	// ロゴ名コピー
	memcpy(adjdata->name,data->name,LOGO_MAX_NAME);

	// 左上座標設定（位置調整済み）
	adjdata->x = data->x +(int)(fp->track[LOGO_X]+200)/4 -50;
	adjdata->y = data->y +(int)(fp->track[LOGO_Y]+200)/4 -50;

	adjdata->w = w = data->w + 1;	// 1/4単位調整するため
	adjdata->h = h = data->h + 1;	// 幅、高さを１増やす

	// LOGO_PIXELの先頭
	(void *)df = (void *)(data +1);
	(void *)ex = (void *)(adjdata +1);

	adjx = (fp->track[LOGO_X]+200) % 4;	// 位置端数
	adjy = (fp->track[LOGO_Y]+200) % 4;

	//----------------------------------------------------- 一番上の列
	ex[0].dp_y  = df[0].dp_y *(4-adjx)*(4-adjy)/16;	// 左端
	ex[0].dp_cb = df[0].dp_cb*(4-adjx)*(4-adjy)/16;
	ex[0].dp_cr = df[0].dp_cr*(4-adjx)*(4-adjy)/16;
	ex[0].y  = df[0].y;
	ex[0].cb = df[0].cb;
	ex[0].cr = df[0].cr;
	for(i=1;i<w-1;i++){									//中
		// Y
		ex[i].dp_y = (df[i-1].dp_y*adjx*(4-adjy)
							+ df[i].dp_y*(4-adjx)*(4-adjy)) /16;
		if(ex[i].dp_y)
			ex[i].y  = (df[i-1].y*Abs(df[i-1].dp_y)*adjx*(4-adjy)
					+ df[i].y * Abs(df[i].dp_y)*(4-adjx)*(4-adjy))
				/(Abs(df[i-1].dp_y)*adjx*(4-adjy) + Abs(df[i].dp_y)*(4-adjx)*(4-adjy));
		// Cb
		ex[i].dp_cb = (df[i-1].dp_cb*adjx*(4-adjy)
							+ df[i].dp_cb*(4-adjx)*(4-adjy)) /16;
		if(ex[i].dp_cb)
			ex[i].cb = (df[i-1].cb*Abs(df[i-1].dp_cb)*adjx*(4-adjy)
					+ df[i].cb * Abs(df[i].dp_cb)*(4-adjx)*(4-adjy))
				/ (Abs(df[i-1].dp_cb)*adjx*(4-adjy)+Abs(df[i].dp_cb)*(4-adjx)*(4-adjy));
		// Cr
		ex[i].dp_cr = (df[i-1].dp_cr*adjx*(4-adjy)
							+ df[i].dp_cr*(4-adjx)*(4-adjy)) /16;
		if(ex[i].dp_cr)
			ex[i].cr = (df[i-1].cr*Abs(df[i-1].dp_cr)*adjx*(4-adjy)
					+ df[i].cr * Abs(df[i].dp_cr)*(4-adjx)*(4-adjy))
				/ (Abs(df[i-1].dp_cr)*adjx*(4-adjy)+Abs(df[i].dp_cr)*(4-adjx)*(4-adjy));
	}
	ex[i].dp_y  = df[i-1].dp_y * adjx *(4-adjy)/16;	// 右端
	ex[i].dp_cb = df[i-1].dp_cb* adjx *(4-adjy)/16;
	ex[i].dp_cr = df[i-1].dp_cr* adjx *(4-adjy)/16;
	ex[i].y  = df[i-1].y;
	ex[i].cb = df[i-1].cb;
	ex[i].cr = df[i-1].cr;

	//----------------------------------------------------------- 中
	for(j=1;j<h-1;j++){
		// 輝度Y		//---------------------- 左端
		ex[j*w].dp_y = (df[(j-1)*data->w].dp_y*(4-adjx)*adjy
						+ df[j*data->w].dp_y*(4-adjx)*(4-adjy)) /16;
		if(ex[j*w].dp_y)
			ex[j*w].y = (df[(j-1)*data->w].y*Abs(df[(j-1)*data->w].dp_y)*(4-adjx)*adjy
						+ df[j*data->w].y*Abs(df[j*data->w].dp_y)*(4-adjx)*(4-adjy))
				/ (Abs(df[(j-1)*data->w].dp_y)*(4-adjx)*adjy+Abs(df[j*data->w].dp_y)*(4-adjx)*(4-adjy));
		// 色差(青)Cb
		ex[j*w].dp_cb = (df[(j-1)*data->w].dp_cb*(4-adjx)*adjy
						+ df[j*data->w].dp_cb*(4-adjx)*(4-adjy)) / 16;
		if(ex[j*w].dp_cb)
			ex[j*w].cb = (df[(j-1)*data->w].cb*Abs(df[(j-1)*data->w].dp_cb)*(4-adjx)*adjy
						+ df[j*data->w].cb*Abs(df[j*data->w].dp_cb)*(4-adjx)*(4-adjy))
				/ (Abs(df[(j-1)*data->w].dp_cb)*(4-adjx)*adjy+Abs(df[j*data->w].dp_cb)*(4-adjx)*(4-adjy));
		// 色差(赤)Cr
		ex[j*w].dp_cr = (df[(j-1)*data->w].dp_cr*(4-adjx)*adjy
						+ df[j*data->w].dp_cr*(4-adjx)*(4-adjy)) / 16;
		if(ex[j*w].dp_cr)
			ex[j*w].cr = (df[(j-1)*data->w].cr*Abs(df[(j-1)*data->w].dp_cr)*(4-adjx)*adjy
						+ df[j*data->w].cr*Abs(df[j*data->w].dp_cr)*(4-adjx)*(4-adjy))
				/ (Abs(df[(j-1)*data->w].dp_cr)*(4-adjx)*adjy+Abs(df[j*data->w].dp_cr)*(4-adjx)*(4-adjy));
		for(i=1;i<w-1;i++){	//------------------ 中
			// Y
			ex[j*w+i].dp_y = (df[(j-1)*data->w+i-1].dp_y*adjx*adjy
							+ df[(j-1)*data->w+i].dp_y*(4-adjx)*adjy
							+ df[j*data->w+i-1].dp_y*adjx*(4-adjy)
							+ df[j*data->w+i].dp_y*(4-adjx)*(4-adjy) ) /16;
			if(ex[j*w+i].dp_y)
				ex[j*w+i].y = (df[(j-1)*data->w+i-1].y*Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy
							+ df[(j-1)*data->w+i].y*Abs(df[(j-1)*data->w+i].dp_y)*(4-adjx)*adjy
							+ df[j*data->w+i-1].y*Abs(df[j*data->w+i-1].dp_y)*adjx*(4-adjy)
							+ df[j*data->w+i].y*Abs(df[j*data->w+i].dp_y)*(4-adjx)*(4-adjy) )
					/ (Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy + Abs(df[(j-1)*data->w+i].dp_y)*(4-adjx)*adjy
						+ Abs(df[j*data->w+i-1].dp_y)*adjx*(4-adjy)+Abs(df[j*data->w+i].dp_y)*(4-adjx)*(4-adjy));
			// Cb
			ex[j*w+i].dp_cb = (df[(j-1)*data->w+i-1].dp_cb*adjx*adjy
							+ df[(j-1)*data->w+i].dp_cb*(4-adjx)*adjy
							+ df[j*data->w+i-1].dp_cb*adjx*(4-adjy)
							+ df[j*data->w+i].dp_cb*(4-adjx)*(4-adjy) ) /16;
			if(ex[j*w+i].dp_cb)
				ex[j*w+i].cb = (df[(j-1)*data->w+i-1].cb*Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy
							+ df[(j-1)*data->w+i].cb*Abs(df[(j-1)*data->w+i].dp_cb)*(4-adjx)*adjy
							+ df[j*data->w+i-1].cb*Abs(df[j*data->w+i-1].dp_cb)*adjx*(4-adjy)
							+ df[j*data->w+i].cb*Abs(df[j*data->w+i].dp_cb)*(4-adjx)*(4-adjy) )
					/ (Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy + Abs(df[(j-1)*data->w+i].dp_cb)*(4-adjx)*adjy
						+ Abs(df[j*data->w+i-1].dp_cb)*adjx*(4-adjy) + Abs(df[j*data->w+i].dp_cb)*(4-adjx)*(4-adjy));
			// Cr
			ex[j*w+i].dp_cr = (df[(j-1)*data->w+i-1].dp_cr*adjx*adjy
							+ df[(j-1)*data->w+i].dp_cr*(4-adjx)*adjy
							+ df[j*data->w+i-1].dp_cr*adjx*(4-adjy)
							+ df[j*data->w+i].dp_cr*(4-adjx)*(4-adjy) ) /16;
			if(ex[j*w+i].dp_cr)
				ex[j*w+i].cr = (df[(j-1)*data->w+i-1].cr*Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy
							+ df[(j-1)*data->w+i].cr*Abs(df[(j-1)*data->w+i].dp_cr)*(4-adjx)*adjy
							+ df[j*data->w+i-1].cr*Abs(df[j*data->w+i-1].dp_cr)*adjx*(4-adjy)
							+ df[j*data->w+i].cr*Abs(df[j*data->w+i].dp_cr)*(4-adjx)*(4-adjy) )
					/ (Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy +Abs(df[(j-1)*data->w+i].dp_cr)*(4-adjx)*adjy
						+ Abs(df[j*data->w+i-1].dp_cr)*adjx*(4-adjy)+Abs(df[j*data->w+i].dp_cr)*(4-adjx)*(4-adjy));
		}
		// Y		//----------------------- 右端
		ex[j*w+i].dp_y = (df[(j-1)*data->w+i-1].dp_y*adjx*adjy
						+ df[j*data->w+i-1].dp_y*adjx*(4-adjy)) / 16;
		if(ex[j*w+i].dp_y)
			ex[j*w+i].y = (df[(j-1)*data->w+i-1].y*Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy
						+ df[j*data->w+i-1].y*Abs(df[j*data->w+i-1].dp_y)*adjx*(4-adjy))
				/ (Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy+Abs(df[j*data->w+i-1].dp_y)*adjx*(4-adjy));
		// Cb
		ex[j*w+i].dp_cb = (df[(j-1)*data->w+i-1].dp_cb*adjx*adjy
						+ df[j*data->w+i-1].dp_cb*adjx*(4-adjy)) / 16;
		if(ex[j*w+i].dp_cb)
			ex[j*w+i].cb = (df[(j-1)*data->w+i-1].cb*Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy
						+ df[j*data->w+i-1].cb*Abs(df[j*data->w+i-1].dp_cb)*adjx*(4-adjy))
				/ (Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy+Abs(df[j*data->w+i-1].dp_cb)*adjx*(4-adjy));
		// Cr
		ex[j*w+i].dp_cr = (df[(j-1)*data->w+i-1].dp_cr*adjx*adjy
						+ df[j*data->w+i-1].dp_cr*adjx*(4-adjy)) / 16;
		if(ex[j*w+i].dp_cr)
			ex[j*w+i].cr = (df[(j-1)*data->w+i-1].cr*Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy
						+ df[j*data->w+i-1].cr*Abs(df[j*data->w+i-1].dp_cr)*adjx*(4-adjy))
				/ (Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy+Abs(df[j*data->w+i-1].dp_cr)*adjx*(4-adjy));
	}
	//--------------------------------------------------------- 一番下
	ex[j*w].dp_y  = df[(j-1)*data->w].dp_y *(4-adjx)*adjy /16;	// 左端
	ex[j*w].dp_cb = df[(j-1)*data->w].dp_cb*(4-adjx)*adjy /16;
	ex[j*w].dp_cr = df[(j-1)*data->w].dp_cr*(4-adjx)*adjy /16;
	ex[j*w].y  = df[(j-1)*data->w].y;
	ex[j*w].cb = df[(j-1)*data->w].cb;
	ex[j*w].cr = df[(j-1)*data->w].cr;
	for(i=1;i<w-1;i++){		// 中
		// Y
		ex[j*w+i].dp_y = (df[(j-1)*data->w+i-1].dp_y * adjx * adjy
								+ df[(j-1)*data->w+i].dp_y * (4-adjx) *adjy) /16;
		if(ex[j*w+i].dp_y)
			ex[j*w+i].y = (df[(j-1)*data->w+i-1].y*Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy
						+ df[(j-1)*data->w+i].y*Abs(df[(j-1)*data->w+i].dp_y)*(4-adjx)*adjy)
				/ (Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy +Abs(df[(j-1)*data->w+i].dp_y)*(4-adjx)*adjy);
		// Cb
		ex[j*w+i].dp_cb = (df[(j-1)*data->w+i-1].dp_cb * adjx * adjy
								+ df[(j-1)*data->w+i].dp_cb * (4-adjx) *adjy) /16;
		if(ex[j*w+i].dp_cb)
			ex[j*w+i].cb = (df[(j-1)*data->w+i-1].cb*Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy
						+ df[(j-1)*data->w+i].cb*Abs(df[(j-1)*data->w+i].dp_cb)*(4-adjx)*adjy )
				/ (Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy +Abs(df[(j-1)*data->w+i].dp_cb)*(4-adjx)*adjy);
		// Cr
		ex[j*w+i].dp_cr = (df[(j-1)*data->w+i-1].dp_cr * adjx * adjy
								+ df[(j-1)*data->w+i].dp_cr * (4-adjx) *adjy) /16;
		if(ex[j*w+i].dp_cr)
			ex[j*w+i].cr = (df[(j-1)*data->w+i-1].cr*Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy
						+ df[(j-1)*data->w+i].cr*Abs(df[(j-1)*data->w+i].dp_cr)*(4-adjx)*adjy)
				/ (Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy +Abs(df[(j-1)*data->w+i].dp_cr)*(4-adjx)*adjy);
	}
	ex[j*w+i].dp_y  = df[(j-1)*data->w+i-1].dp_y *adjx*adjy /16;	// 右端
	ex[j*w+i].dp_cb = df[(j-1)*data->w+i-1].dp_cb*adjx*adjy /16;
	ex[j*w+i].dp_cr = df[(j-1)*data->w+i-1].dp_cr*adjx*adjy /16;
	ex[j*w+i].y  = df[(j-1)*data->w+i-1].y;
	ex[j*w+i].cb = df[(j-1)*data->w+i-1].cb;
	ex[j*w+i].cr = df[(j-1)*data->w+i-1].cr;

	return TRUE;
}


/*--------------------------------------------------------------------
*	update_combobox()		コンボボックスの選択を更新
*		拡張データ領域のロゴ名にコンボボックスのカーソルを合わせる
*-------------------------------------------------------------------*/
static void update_cb_logo(char *name)
{
	int num;

	// コンボボックス検索
	num = SendMessage(dialog.cb_logo,CB_FINDSTRING,-1,(WPARAM)name);

	if(num == CB_ERR)	// みつからなかった
		num = -1;

	SendMessage(dialog.cb_logo,CB_SETCURSEL,num,0);	// カーソルセット
}

/*--------------------------------------------------------------------
*	change_param()		パラメータの変更
*		選択されたロゴデータを拡張データ領域にコピー
*-------------------------------------------------------------------*/
static void change_param(void)
{
	LRESULT ret;

	// 選択番号取得
	ret = SendMessage(dialog.cb_logo,CB_GETCURSEL,0,0);
	ret = SendMessage(dialog.cb_logo,CB_GETITEMDATA,ret,0);

	if(ret!=CB_ERR)
		memcpy(ex_data,(void *)ret,LOGO_MAX_NAME);	// ロゴ名をコピー
}

/*--------------------------------------------------------------------
*	set_cb_logo()
*		外部ファイルからロゴを読み込み、セット
*-------------------------------------------------------------------*
static void set_cb_logo(FILTER *fp)
{
	ULONG   ptr;
	HANDLE  hFile;

	// INIからロゴデータファイル名を読み込む
	fp->exfunc->ini_load_str(fp,LDP_KEY,logodata_file,NULL);

	if(lstrlen(logodata_file)==0){	// ロゴデータファイル名がなかったとき
		// 読み込みダイアログ
		if(!fp->exfunc->dlg_get_load_name(logodata_file,LDP_FILTER,LDP_DEFAULT)){
			// キャンセルされた
			MessageBox(fp->hwnd,"ロゴデータファイルがありません",filter_name,MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else{	// ロゴデータファイル名があるとき
		// 存在を調べる
		hFile = CreateFile(logodata_file, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

		if(hFile==INVALID_HANDLE_VALUE){	// みつからなかったとき
			MessageBox(fp->hwnd,"ロゴデータファイルが見つかりません",filter_name,MB_OK|MB_ICONWARNING);
			if(!fp->exfunc->dlg_get_load_name(logodata_file,LDP_FILTER,LDP_DEFAULT)){
				// キャンセルされた
				MessageBox(fp->hwnd,"ロゴデータファイルがありません",filter_name,MB_OK|MB_ICONWARNING);
				return;
			}
		}
		else
			CloseHandle(hFile);
	}

	// ロゴファイル読み込み
	read_logo_pack(logodata_file,fp);

	if(SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0))
		// 拡張データ初期値設定
		fp->ex_data_def = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,0,0);
}

/*--------------------------------------------------------------------
* 	set_combo_item()		コンボボックスにアイテムをセット
* 			dataはmallocで確保したポインタ
* 			個別にlogodata配列に書き込む必要がある
*-------------------------------------------------------------------*/
static int set_combo_item(void* data)
{
	int num;

	// コンボボックスアイテム数
	num = SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0);

	// 最後尾に追加
	SendMessage(dialog.cb_logo,CB_INSERTSTRING,num,(LPARAM)data);
	SendMessage(dialog.cb_logo,CB_SETITEMDATA,num,(LPARAM)data);

	return num;
}

/*--------------------------------------------------------------------
*	del_combo_item()		コンボボックスからアイテムを削除
*-------------------------------------------------------------------*/
static void del_combo_item(int num)
{
	void *ptr;
	unsigned int i;

	ptr = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,num,0);
	if(ptr) free(ptr);

	// ロゴデータ配列再構成
	logodata_n = SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0);
	logodata = realloc(logodata,logodata_n*sizeof(logodata));

	for(i=0;i<logodata_n;i++)
		logodata[i] = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,i,0);

	SendMessage(dialog.cb_logo,CB_DELETESTRING,num,0);
}

/*--------------------------------------------------------------------
* 	read_logo_pack()		ロゴデータ読み込み
* 		ファイルからロゴデータを読み込み
* 		コンボボックスにセット
* 		拡張領域にコピー
*-------------------------------------------------------------------*/
static void read_logo_pack(char *fname,FILTER *fp)
{
	HANDLE hFile;
	LOGO_HEADER lgh;
	DWORD readed = 0;
	ULONG ptr;
	void* data;
	unsigned char num;	// ファイルに含まれるデータの数
	int i;
	int same;
	char message[255];

	// ファイルオープン
	hFile = CreateFile(fname,GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile==INVALID_HANDLE_VALUE){
		MessageBox(fp->hwnd,"ロゴデータファイルが見つかりません",filter_name,MB_OK|MB_ICONERROR);
		return;
	}
	if(GetFileSize(hFile, NULL)<sizeof(LOGO_HEADER)){	// サイズ確認
		CloseHandle(hFile);
		MessageBox(fp->hwnd,"ロゴデータファイルが不正です",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	SetFilePointer(hFile,31, 0, FILE_BEGIN);	// 先頭から31byteへ
	ReadFile(hFile,&num,1,&readed,NULL);	// データ数取得

	logodata_n = 0;	// 書き込みデータカウンタ
	logodata = NULL;

	for(i=0;i<num;i++){

		// LOGO_HEADER 読み込み
		readed = 0;
		ReadFile(hFile,&lgh,sizeof(LOGO_HEADER),&readed, NULL);
		if(readed!=sizeof(LOGO_HEADER)){
			MessageBox(fp->hwnd,"ロゴデータの読み込みに失敗しました",filter_name,MB_OK|MB_ICONERROR);
			break;
		}

//  ldpには基本的に同名のロゴは存在しない
//
//		// 同名ロゴがあるか
//		same = find_logodata(lgh.name);
//		if(same>0){
//			wsprintf(message,"同名のロゴがあります\n置き換えますか？\n\n%s",lgh.name);
//			if(MessageBox(fp->hwnd,message,filter_name,MB_YESNO|MB_ICONQUESTION)==IDYES){
//				// 削除
//				del_combo_item(same);
//			}
//			else{	// 上書きしない
//				// ファイルポインタを進める
//				SetFilePointer(hFile,LOGO_PIXELSIZE(&lgh), 0, FILE_CURRENT);
//				continue;
//			}
//		}

		// メモリ確保
		data = malloc(LOGO_DATASIZE(&lgh));
		if(data==NULL){
			MessageBox(fp->hwnd,"メモリが足りません",filter_name,MB_OK|MB_ICONERROR);
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

		// logodataポインタ配列に追加
		logodata_n++;
		logodata = realloc(logodata,logodata_n*sizeof(logodata));
		logodata[logodata_n-1] = data;
	}

	CloseHandle(hFile);

	if(logodata_n){
		// 拡張データ設定
		memcpy(ex_data,logodata[0],LOGO_MAX_NAME);
	}
}

/*--------------------------------------------------------------------
*	on_option_button()		オプションボタン動作
*-------------------------------------------------------------------*/
static BOOL on_option_button(FILTER* fp)
{
	LRESULT res;
	void   *data;
	unsigned int i;

	// オプションダイアログが参照する
	optfp = fp;
	hcb_logo = dialog.cb_logo;

	EnableWindow(dialog.bt_opt,FALSE);	// オプションボタン無効化

	// オプションダイアログ表示（モーダルフレーム）
	res = DialogBox(fp->dll_hinst,"OPT_DLG",GetWindow(fp->hwnd, GW_OWNER),OptDlgProc);

	EnableWindow(dialog.bt_opt,TRUE);	// 有効に戻す

	if(res==IDOK){	// OKボタン
		logodata_n = SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0);

		// logodata配列再構成
		logodata = realloc(logodata,logodata_n*sizeof(logodata));
		for(i=0;i<logodata_n;i++)
			logodata[i] = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,i,0);

		if(logodata_n)	// 拡張データ初期値設定
			fp->ex_data_def = logodata[0];
		else
			fp->ex_data_def = NULL;
	}

	return TRUE;
}

/*--------------------------------------------------------------------
*	set_sended_data()		受信したロゴデータをセット
*-------------------------------------------------------------------*/
static void set_sended_data(void* data,FILTER* fp)
{
	void *ptr;
	UINT same;
	char message[256];
	LOGO_HEADER *lgh;

	lgh = (LOGO_HEADER *)data;

	// 同名のロゴがあるかどうか
	same = SendMessage(dialog.cb_logo,CB_FINDSTRING,-1,(WPARAM)lgh->name);
	if(same!=CB_ERR){
		wsprintf(message,"同名のロゴがあります\n置き換えますか？\n\n%s",data);
		if(MessageBox(fp->hwnd,message,filter_name,MB_YESNO|MB_ICONQUESTION)!=IDYES)
			return;	// 上書きしない

		del_combo_item(same);
	}

	ptr = malloc(LOGO_DATASIZE(lgh));
	if(ptr==NULL){
		MessageBox(fp->hwnd,"メモリが確保できませんでした",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	memcpy(ptr,data,LOGO_DATASIZE(data));

	logodata_n++;
	logodata = realloc(logodata,logodata_n*sizeof(logodata));

	logodata[logodata_n-1] = ptr;
	set_combo_item(ptr);
}

//*/
