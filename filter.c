/*********************************************************************
* 	透過性ロゴ（BSマークとか）除去フィルタ
* 								ver 0.09a
* 
* 2003
* 	02/01:	製作開始
* 	02/02:	拡張データ領域を使うとプロファイル切り替えた時AviUtlがくたばる。
* 	      	なぜ？バグ？どうしよう。
* 	      	と思ったら、領域サイズに制限があるのね… SDKには一言も書いてないけど。
* 	      	消えた－－!!（Ｂだけ）ちょっと感動。
* 	      	BSロゴって輝度だけの変化なのか？RGBでやると色変わるし。
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
* 			プレビューで枠からはみ出さないようにした。（はみ出しすぎると落ちる）
* 	04/20:	フィルタ名変更。ロゴ付加モードを一時廃止。
* 	04/28:	1/4単位位置調整実装。
* 			ロゴ付加モード(あまり意味ないけど)復活
* 			オプションダイアログ表示中にAviUtlを終了できないように変更
* 			（エラーを出して落ちるバグ回避）
* 
* [正式版(0.01)公開]
* 
* 	05/04:	不透明度調整の方法を変更。
* 	05/08:	メモリ関連ルーチン大幅変更		(0.02)
* 			VFAPI動作に対応、プロファイルの途中切り替えに対応
* 			ロゴデータのサイズ制限を約４倍にした。
* 	05/10:	データが受信できなくなっていたバグを修正	(0.03)
* 	05/17:	ロゴ名を編集できるようにした。(0.04)
* 	06/12:	プレビューの背景色をRGBで指定できるように変更。
* 			位置調整が４の倍数のときcreate_adj_exdata()を呼ばないようにした。（0.05）
* 	06/30:	フェードイン・アウトに対応。 (0.06)
* 	07/02:	ロゴデータを受信できない場合があったのを修正。
* 	07/03:	YCbCrの範囲チェックをするようにした。(しないと落ちることがある)
* 			ロゴ名編集で同名にせっていできないようにした。(0.06a)
* 	08/01:	フェードの不透明度計算式を見直し
* 	08/02:	実数演算を止め、無駄な演算を削除して高速化。
* 			上に伴い深度のデフォルト値を変更。
* 			細かな修正
* 	09/05:	細かな修正
* 	09/27:	filter.hをAviUtl0.99SDKのものに差し替え。(0.07)
* 	10/20:	SSE2使用のrgb2ycがバグもちなので、自前でRGB->YCbCrするようにした。
* 			位置X/Yの最大･最小値を拡張した。(0.07a)
* 	10/25:	位置調整で-200以下にすると落ちるバグ修正。(0.07b)
* 2004
* 	02/18:	AviSynthスクリプトを吐くボタン追加。(0.08)
* 	04/17:	ロゴデータファイル読み込み時にデータが一つも無い時エラーを出さないようにした。
* 			開始･終了の最大値を4096まで増やした。(0.08a)
* 	09/19:	スタックを無駄遣いしていたのを修正。
* 			開始・フェードイン・アウト・終了の初期値をロゴデータに保存できるようにした。(0.09)
* 2005
* 	04/18:	フィルタ名、パラメタ名を変更できるようにした。(0.09a)
* 
*********************************************************************/

/* ToDo:
* 	・ロゴデータの作成・編集機能
* 	・ロゴデータに開始･終了･フェードの情報埋め込み
* 		コンボボックスで変更された時だけそれらを反映させる（たくや氏よ、プロファイル使えば必要ないでしょうに
* 
*  MEMO:
* 	・ロゴの拡大縮小ルーチン自装しないとだめかなぁ。
* 		→必要なさげ。当面は自装しない。
* 	・ロゴ作成・編集は別アプリにしてしまおうか…
* 		仕様公開してるし、誰か作ってくれないかなぁ（他力本願）
* 	・ロゴ除去モードとロゴ付加モードを切り替えられるようにしようかな
* 		→付けてみた
* 	・解析プラグからデータを受け取るには…独自WndMsg登録してSendMessageで送ってもらう
* 		→ちゃんと動いた。…登録しなくてもUSER定義でよかったかも
* 	・ロゴに１ピクセル未満のズレがある。1/4ピクセルくらいでの位置調整が必要そう。
* 		→実装完了
* 	・ダイアログを表示したまま終了するとエラー吐く
* 		→親ウィンドウをAviUtl本体にすることで終了できなくした
* 	・ロゴデータ構造少し変えようかな… 色差要素のビットを半分にするとか。
* 
*  新メモリ管理について:(2003/05/08)
* 	fp->ex_data_ptrにはロゴの名称のみを保存。（7FFDバイトしかプロファイルに保存されず、不具合が生じるため）
* 	func_init()でロゴデータパックを読み込む。ldpファイル名は必ずフルパスであることが必要。
* 	読み込んだロゴデータのポインタはlogodata配列に保存。配列のデータ数はlogodata_nに。
* 	func_proc()ではex_data（ロゴ名称）と一致するデータをlogodata配列から検索。なかった場合は何もしない。
* 	位置パラメータを使って位置調整データを作成。その後で除去・付加関数を呼ぶ。
* 	WndProcでは、WM_FILTER_INITでコンボボックスアイテムをlogodata配列から作る。
* 	コンボアイテムのITEMDATAには従来どおりロゴデータのポインタを保存する。
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
#include "strdlg.h"
#include "logodef.h"


#define ID_BUTTON_OPTION 40001
#define ID_COMBO_LOGO    40002
#define ID_BUTTON_SYNTH  40003

#define Abs(x) ((x>0)? x:-x)
#define Clamp(n,l,h) ((n<l) ? l : (n>h) ? h : n)


#define LDP_KEY     "logofile"
#define LDP_DEFAULT "logodata.ldp"
#define LDP_FILTER  "ロゴデータパック (*.ldp)\0*.ldp\0"\
                    "AllFiles (*.*)\0*.*\0"


// ダイアログアイテム
typedef struct {
	HFONT font;
	HWND  cb_logo;
	HWND  bt_opt;
	HWND  bt_synth;
} FILTER_DIALOG;

FILTER_DIALOG dialog;

char  logodata_file[MAX_PATH] = { 0 };	// ロゴデータファイル名(INIに保存)

LOGO_HEADER** logodata   = NULL;
unsigned int  logodata_n = 0;

char ex_data[LOGO_MAX_NAME];	// 拡張データ領域

static UINT  WM_SEND_LOGO_DATA =0;	// ロゴ受信メッセージ


//----------------------------
//	プロトタイプ宣言
//----------------------------
static void on_wm_filter_init(FILTER* fp);
static void on_wm_filter_exit(FILTER* fp);
static void init_dialog(HWND hwnd,HINSTANCE hinst);
static void update_cb_logo(char *name);
static void change_param(FILTER* fp);
static void set_cb_logo(FILTER* fp);
static int  set_combo_item(void* data);
static void del_combo_item(int num);
static void read_logo_pack(char *logodata_file,FILTER *fp);
static void set_sended_data(void* logodata,FILTER* fp);
static BOOL create_adj_exdata(FILTER *fp,LOGO_HEADER *adjdata,const LOGO_HEADER *data);
static int  find_logo(const char *logo_name);

static BOOL on_option_button(FILTER* fp);
static BOOL on_avisynth_button(FILTER* fp,void* editp);

BOOL func_proc_eraze_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh,int);
BOOL func_proc_add_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh,int);

//----------------------------
//	FILTER_DLL構造体
//----------------------------
char filter_name[] = LOGO_FILTER_NAME;
char filter_info[] = LOGO_FILTER_NAME" ver 0.09a by MakKi";
#define track_N 10
#if track_N
TCHAR *track_name[]   = { 	"位置 X", "位置 Y", 
							"深度", "Y", "Cb", "Cr", 
							"開始", "FadeIn", "FadeOut", "終了" };	// トラックバーの名前
int   track_default[] = {           0,           0, 128,    0,    0,    0, 0, 0, 0, 0 };	// トラックバーの初期値
int   track_s[]       = { LOGO_XY_MIN, LOGO_XY_MIN,   0, -100, -100, -100, 0, 0, 0, 0 };	// トラックバーの下限値
int   track_e[]       = { LOGO_XY_MAX, LOGO_XY_MAX, 256,  100,  100,  100, LOGO_STED_MAX, LOGO_FADE_MAX, LOGO_FADE_MAX, LOGO_STED_MAX };	// トラックバーの上限値
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
#define LOGO_STRT   6
#define LOGO_FIN    7
#define LOGO_FOUT   8
#define LOGO_END    9

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
			if(logodata[i]){
				free(logodata[i]);
				logodata[i] = NULL;
			}
		}
		free(logodata);
		logodata = NULL;
	}

	return TRUE;
}

/*====================================================================
*	フィルタ処理関数
*===================================================================*/
BOOL func_proc(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	static char adjdata[LOGO_MAXSIZE];
	int num;
	int fade;
	int s,e;

	// ロゴ検索
	num = find_logo(fp->ex_data_ptr);
	if(num<0) return FALSE;

	// 選択範囲取得
	if(!fp->exfunc->get_select_frame(fpip->editp,&s,&e))
		return FALSE;

	// フェード不透明度計算
	if(fpip->frame < s+fp->track[LOGO_STRT]+fp->track[LOGO_FIN]){
		if(fpip->frame < s+fp->track[LOGO_STRT])
			return FALSE;	// フェードイン前
		else	// フェードイン
			fade = ((fpip->frame-s-fp->track[LOGO_STRT])*2 +1)*LOGO_FADE_MAX / (fp->track[LOGO_FIN]*2);
	}
	else if(fpip->frame > e-fp->track[LOGO_FOUT]-fp->track[LOGO_END]){
		if(fpip->frame > e-fp->track[LOGO_END])
			return FALSE;	// フェードアウト後
		else	// フェードアウト
			fade = ((e-fpip->frame-fp->track[LOGO_END])*2+1)*LOGO_FADE_MAX / (fp->track[LOGO_FOUT]*2);
	}
	else
		fade = LOGO_FADE_MAX;	// 通常


	if(fp->track[LOGO_X]%4 || fp->track[LOGO_Y]%4){
		// 位置調整が４の倍数でないとき、1/4ピクセル単位調整
		if(!create_adj_exdata(fp,(void *)adjdata,logodata[num]))
			return FALSE;
	}
	else {
		// 4の倍数のときはx,yのみ書き換え
		memcpy(adjdata,logodata[num],LOGO_DATASIZE(logodata[num]));
		((LOGO_HEADER *)adjdata)->x += fp->track[LOGO_X] / 4;
		((LOGO_HEADER *)adjdata)->y += fp->track[LOGO_Y] / 4;
	}

	if(fp->check[1])	// 除去モードチェック
		return func_proc_eraze_logo(fp,fpip,(void *)adjdata,fade);	// ロゴ除去モード
	else
		return func_proc_add_logo(fp,fpip,(void *)adjdata,fade);	// ロゴ付加モード
}

/*--------------------------------------------------------------------
* 	func_proc_eraze_logo()	ロゴ除去モード
*-------------------------------------------------------------------*/
BOOL func_proc_eraze_logo(FILTER* fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh,int fade)
{
	int   i,j;
	int   yc;
	int   dp;
	PIXEL_YC    *ptr;
	LOGO_PIXEL  *lgp;


	// LOGO_PIXELデータへのポインタ
	(void *)lgp = lgh + 1;

	// 左上の位置へ移動
	ptr = fpip->ycp_edit;
	ptr += lgh->x + lgh->y * fpip->max_w;

	for(i=0;i < lgh->h;++i){
		for(j=0;j < lgh->w;++j){

			if(ptr >= fpip->ycp_edit &&	// 画面内の時のみ処理
			   ptr < fpip->ycp_edit + fpip->max_w*fpip->h)
			{
				// 輝度
				dp = (lgp->dp_y * fp->track[LOGO_YDP] * fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					if(dp==LOGO_MAX_DP) dp--;	// 0での除算回避
					yc = lgp->y + fp->track[LOGO_PY]*16;
					yc = (ptr->y*LOGO_MAX_DP - yc*dp +(LOGO_MAX_DP-dp)/2) /(LOGO_MAX_DP - dp);	// 逆算
					ptr->y = Clamp(yc,-128,4096+128);
				}

				// 色差(青)
				dp = (lgp->dp_cb * fp->track[LOGO_CBDP] * fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					if(dp==LOGO_MAX_DP) dp--;	// 0での除算回避
					yc = lgp->cb + fp->track[LOGO_CB]*16;
					yc = (ptr->cb*LOGO_MAX_DP - yc*dp +(LOGO_MAX_DP-dp)/2) /(LOGO_MAX_DP - dp);
					ptr->cb = Clamp(yc,-2048-128,2048+128);
				}

				// 色差(赤)
				dp = (lgp->dp_cr * fp->track[LOGO_CRDP] * fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					if(dp==LOGO_MAX_DP) dp--;	// 0での除算回避
					yc = lgp->cr + fp->track[LOGO_CR]*16;
					yc = (ptr->cr*LOGO_MAX_DP - yc*dp +(LOGO_MAX_DP-dp)/2) /(LOGO_MAX_DP - dp);
					ptr->cr = Clamp(yc,-2048-128,2048+128);
				}

			}	// if画面内

			++ptr;	// 隣りへ
			++lgp;
		}
		// 次のラインへ
		ptr += fpip->max_w - j;
	}

	return TRUE;
}

/*--------------------------------------------------------------------
* 	func_proc_add_logo()	ロゴ付加モード
*-------------------------------------------------------------------*/
BOOL func_proc_add_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh,int fade)
{
	int   i,j;
	int   yc;
	PIXEL_YC    *ptr;
	LOGO_PIXEL  *lgp;
	int  dp;


	// LOGO_PIXELデータへのポインタ
	(void *)lgp = lgh + 1;

	// 左上の位置へ移動
	ptr = fpip->ycp_edit;
	ptr += lgh->x + lgh->y * fpip->max_w;

	for(i=0;i < lgh->h;++i){
		for(j=0;j < lgh->w;++j){

			if(ptr >= fpip->ycp_edit &&	// 画面内の時のみ処理
			   ptr < fpip->ycp_edit + fpip->max_w*fpip->h)
			{
				// 輝度
				dp = (lgp->dp_y * fp->track[LOGO_YDP] *fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					yc = lgp->y    + fp->track[LOGO_PY]*16;
					yc = (ptr->y*(LOGO_MAX_DP-dp) + yc*dp +(LOGO_MAX_DP/2)) /LOGO_MAX_DP;	// ロゴ付加
					ptr->y = Clamp(yc,-128,4096+128);
				}


				// 色差(青)
				dp = (lgp->dp_cb * fp->track[LOGO_CBDP] *fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					yc = lgp->cb   + fp->track[LOGO_CB]*16;
					yc = (ptr->cb*(LOGO_MAX_DP-dp) + yc*dp +(LOGO_MAX_DP/2)) /LOGO_MAX_DP;
					ptr->cb = Clamp(yc,-2048-128,2048+128);
				}

				// 色差(赤)
				dp = (lgp->dp_cr * fp->track[LOGO_CRDP] * fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					yc = lgp->cr   + fp->track[LOGO_CR]*16;
					yc = (ptr->cr*(LOGO_MAX_DP-dp) + yc*dp +(LOGO_MAX_DP/2)) /LOGO_MAX_DP;
					ptr->cr = Clamp(yc,-2048-128,2048+128);
				}

			}	// if画面内

			++ptr;	// 隣りへ
			++lgp;
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

	for(i=0;i<logodata_n;++i){
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
							change_param(fp);
							return TRUE;
					}
					break;

				case ID_BUTTON_SYNTH:	// AviSynthボタン
					return on_avisynth_button(fp,editp);
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
	LOGO_FILE_HEADER lfh;

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
	ZeroMemory(&lfh,sizeof(lfh));
	strcpy(lfh.str,LOGO_FILE_HEADER_STR);
	dw = 0;
	WriteFile(hFile,&lfh,sizeof(LOGO_FILE_HEADER),&dw,NULL);
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

		lfh.logonum = n;
		SetFilePointer(hFile,0, 0, FILE_BEGIN);	// 先頭へ
		dw = 0;
		WriteFile(hFile,&lfh,sizeof(lfh),&dw,NULL);
		if(dw!=sizeof(lfh))
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

	// AviSynthボタン
	dialog.bt_synth = CreateWindow("BUTTON", "AviSynth", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_VCENTER,
									240,ITEM_Y-25, 63,20, hwnd, (HMENU)ID_BUTTON_SYNTH, hinst, NULL);
	SendMessage(dialog.bt_synth, WM_SETFONT, (WPARAM)dialog.font, 0);
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

	// 左上座標設定（位置調整後）
	adjdata->x = data->x +(int)(fp->track[LOGO_X]-LOGO_XY_MIN)/4 + LOGO_XY_MIN/4;
	adjdata->y = data->y +(int)(fp->track[LOGO_Y]-LOGO_XY_MIN)/4 + LOGO_XY_MIN/4;

	adjdata->w = w = data->w + 1;	// 1/4単位調整するため
	adjdata->h = h = data->h + 1;	// 幅、高さを１増やす

	// LOGO_PIXELの先頭
	(void *)df = (void *)(data +1);
	(void *)ex = (void *)(adjdata +1);

	adjx = (fp->track[LOGO_X]-LOGO_XY_MIN) % 4;	// 位置端数
	adjy = (fp->track[LOGO_Y]-LOGO_XY_MIN) % 4;

	//----------------------------------------------------- 一番上の列
	ex[0].dp_y  = df[0].dp_y *(4-adjx)*(4-adjy)/16;	// 左端
	ex[0].dp_cb = df[0].dp_cb*(4-adjx)*(4-adjy)/16;
	ex[0].dp_cr = df[0].dp_cr*(4-adjx)*(4-adjy)/16;
	ex[0].y  = df[0].y;
	ex[0].cb = df[0].cb;
	ex[0].cr = df[0].cr;
	for(i=1;i<w-1;++i){									//中
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
	for(j=1;j<h-1;++j){
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
		for(i=1;i<w-1;++i){	//------------------ 中
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
	for(i=1;i<w-1;++i){		// 中
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
static void change_param(FILTER* fp)
{
	LRESULT ret;

	// 選択番号取得
	ret = SendMessage(dialog.cb_logo,CB_GETCURSEL,0,0);
	ret = SendMessage(dialog.cb_logo,CB_GETITEMDATA,ret,0);

	if(ret!=CB_ERR)
		memcpy(ex_data,(void *)ret,LOGO_MAX_NAME);	// ロゴ名をコピー

	// 開始･フェードイン･アウト･終了の初期値があるときはパラメタに反映
	ret = find_logo((char *)ret);
	if(ret<0) return;

	if(logodata[ret]->fi || logodata[ret]->fo || logodata[ret]->st || logodata[ret]->ed){
		fp->track[LOGO_STRT] = logodata[ret]->st;
		fp->track[LOGO_FIN]  = logodata[ret]->fi;
		fp->track[LOGO_FOUT] = logodata[ret]->fo;
		fp->track[LOGO_END]  = logodata[ret]->ed;
		fp->exfunc->filter_window_update(fp);
	}
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
	LOGO_FILE_HEADER lfh;
	LOGO_HEADER lgh;
	DWORD readed = 0;
	ULONG ptr;
	void* data;
	int i;
	int same;
//	char message[255];

	// ファイルオープン
	hFile = CreateFile(fname,GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile==INVALID_HANDLE_VALUE){
		MessageBox(fp->hwnd,"ロゴデータファイルが見つかりません",filter_name,MB_OK|MB_ICONERROR);
		return;
	}
	if(GetFileSize(hFile, NULL)<sizeof(LOGO_FILE_HEADER)){	// サイズ確認
		CloseHandle(hFile);
		MessageBox(fp->hwnd,"ロゴデータファイルが不正です",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

//	SetFilePointer(hFile,31, 0, FILE_BEGIN);	// 先頭から31byteへ
	ReadFile(hFile,&lfh,sizeof(LOGO_FILE_HEADER),&readed,NULL);	// ファイルヘッダ取得

	logodata_n = 0;	// 書き込みデータカウンタ
	logodata = NULL;

	for(i=0;i<lfh.logonum;i++){

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
	static char message[256];
	void *ptr;
	UINT same;
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

	lstrcpy(fp->ex_data_ptr,ptr);	// 拡張領域にロゴ名をコピー
}


/*--------------------------------------------------------------------
*	on_avisynth_button()	AviSynthボタン動作
*-------------------------------------------------------------------*/
static BOOL on_avisynth_button(FILTER* fp,void *editp)
{
	static char str[STRDLG_MAXSTR];
	int  s,e;

	// スクリプト生成
	wsprintf(str,"%sLOGO(logofile=\"%s\",\r\n"
	             "\\           logoname=\"%s\"",
				(fp->check[0]? "Add":"Erase"),logodata_file,fp->ex_data_ptr);

	if(fp->track[LOGO_X] || fp->track[LOGO_Y])
		wsprintf(str,"%s,\r\n\\           pos_x=%d, pos_y=%d",
					str,fp->track[LOGO_X],fp->track[LOGO_Y]);

	if(fp->track[LOGO_YDP]!=128 || fp->track[LOGO_PY] || fp->track[LOGO_CB] || fp->track[LOGO_CR])
		wsprintf(str,"%s,\r\n\\           depth=%d, yc_y=%d, yc_u=%d, yc_v=%d",
					str,fp->track[LOGO_YDP],fp->track[LOGO_PY],fp->track[LOGO_CB],fp->track[LOGO_CR]);


	if(fp->exfunc->get_frame_n(editp)){	// 画像が読み込まれているとき
		fp->exfunc->get_select_frame(editp,&s,&e);	// 選択範囲取得
		wsprintf(str,"%s,\r\n\\           start=%d",str, s+fp->track[LOGO_STRT]);

		if(fp->track[LOGO_FIN] || fp->track[LOGO_FOUT])
			wsprintf(str,"%s, fadein=%d, fadeout=%d",str,fp->track[LOGO_FIN],fp->track[LOGO_FOUT]);

		wsprintf(str,"%s, end=%d",str,e-fp->track[LOGO_END]);
	}
	else{
		if(fp->track[LOGO_FIN] || fp->track[LOGO_FOUT])
			wsprintf(str,"%s,\r\n\\           fadein=%d, fadeout=%d",str,fp->track[LOGO_FIN],fp->track[LOGO_FOUT]);
	}

	wsprintf(str,"%s)\r\n",str);

	
	EnableWindow(dialog.bt_synth,FALSE);	// synthボタン無効化

	// ダイアログ呼び出し
	DialogBoxParam(fp->dll_hinst,"STR_DLG",GetWindow(fp->hwnd,GW_OWNER),StrDlgProc,(LPARAM)str);

	EnableWindow(dialog.bt_synth,TRUE);	// synthボタン無効化解除

	return TRUE;
}



/*********************************************************************
*	DLLMain
*********************************************************************/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
#define TRACK_N track_N
#define CHECK_N check_N
#define FILTER_NAME_MAX  32
#define FILTER_TRACK_MAX 16
#define FILTER_CHECK_MAX 32

  //FILTER filter = ::filter;
  static char *strings[1+TRACK_N+CHECK_N];
  char key[16];
  char ini_name[MAX_PATH];
  int i;

  switch(fdwReason){
    case DLL_PROCESS_ATTACH:	// 開始時
      // iniファイル名を取得
      GetModuleFileName(hinstDLL,ini_name,MAX_PATH-4);
      strcat(ini_name,".ini");

      // フィルタ名
      strings[0] = malloc(FILTER_NAME_MAX);
      if(strings[0]==NULL) break;
      GetPrivateProfileString("string","name",filter.name,strings[0],FILTER_NAME_MAX,ini_name);
      filter.name = strings[0];

      // トラック名
      for(i=0;i<TRACK_N;i++){
        strings[i+1] = malloc(FILTER_TRACK_MAX);
        if(strings[i+1]==NULL) break;
        wsprintf(key,"track%d",i);
        GetPrivateProfileString("string",key,filter.track_name[i],strings[i+1],FILTER_TRACK_MAX,ini_name);
        filter.track_name[i] = strings[i+1];
      }

      // チェック名
      for(i=0;i<CHECK_N;i++){
        strings[i+TRACK_N+1] = malloc(FILTER_CHECK_MAX);
        if(strings[i+TRACK_N+1]==NULL) break;
        wsprintf(key,"check%d",i);
        GetPrivateProfileString("string",key,filter.check_name[i],strings[i+TRACK_N+1],FILTER_CHECK_MAX,ini_name);
        filter.check_name[i] = strings[i+TRACK_N+1];
      }
      break;

    case DLL_PROCESS_DETACH:	// 終了時
      // stringsを破棄
      for(i=0;i<1+TRACK_N+CHECK_N && strings[i];i++)
        free(strings[i]);
      break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
      break;
  }
  return TRUE;
}
//*/
