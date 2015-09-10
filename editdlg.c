/*====================================================================
* 	編集ダイアログ			editdlg.c
*===================================================================*/
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "editdlg.h"
#include "logo.h"
#include "optdlg.h"

extern char filter_name[];	// フィルタ名[filter.c]

static HWND owner;	// 親ウインドウ
static int  list_n;

//----------------------------
//	関数プロトタイプ
//----------------------------
void on_wm_initdialog(HWND hdlg);
BOOL on_IDOK(HWND hdlg);

/*====================================================================
* 	EditDlgProc()		コールバックプロシージャ
*===================================================================*/
BOOL CALLBACK EditDlgProc(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg){
		case WM_INITDIALOG:
			owner = GetWindow(hdlg,GW_OWNER);
			list_n = (int)lParam;
			on_wm_initdialog(hdlg);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDOK:
					if(on_IDOK(hdlg))
						EndDialog(hdlg,LOWORD(wParam));
					break;

				case IDCANCEL:
					EndDialog(hdlg,LOWORD(wParam));
					break;
			}
	}

	return FALSE;
}


/*--------------------------------------------------------------------
* 	on_wm_initdialog()	初期化
*-------------------------------------------------------------------*/
void on_wm_initdialog(HWND hdlg)
{
	char title[LOGO_MAX_NAME+10];

	// ロゴ名取得
	SendDlgItemMessage(owner,IDC_LIST,LB_GETTEXT,list_n,(LPARAM)title);

	// ロゴ名エディットボックス
	SendDlgItemMessage(hdlg,ID_EDIT_NAME,EM_SETLIMITTEXT,LOGO_MAX_NAME-1,0);
	SetDlgItemText(hdlg,ID_EDIT_NAME,title);

	// キャプション
	wsprintf(title,"%s - 編集",title);
	SetWindowText(hdlg,title);
}

/*--------------------------------------------------------------------
* 	on_IDOK()	OKボタン動作
* 		ロゴ名の変更
*-------------------------------------------------------------------*/
BOOL on_IDOK(HWND hdlg)
{
	LOGO_HEADER *newdata;
	LOGO_HEADER *olddata;

	char newname[LOGO_MAX_NAME];
	int  num;

	// 新ロゴ名前
	GetDlgItemText(hdlg,ID_EDIT_NAME,newname,LOGO_MAX_NAME);
	// リストボックスを検索
	num = SendDlgItemMessage(owner,IDC_LIST,LB_FINDSTRING,-1,(WPARAM)newname);
	if(num!=CB_ERR){	// 同名が見つかった
		MessageBox(hdlg,"同名のロゴがあります\n別の名称を設定してください",filter_name,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	olddata = (LOGO_HEADER *)SendDlgItemMessage(owner,IDC_LIST,LB_GETITEMDATA,list_n,0);

	// メモリ確保
	newdata = (LOGO_HEADER *)malloc(LOGO_DATASIZE(olddata));
	if(newdata==NULL){
		MessageBox(hdlg,"メモリが確保できませんでした",filter_name,MB_OK|MB_ICONERROR);
		return TRUE;
	}
	// ロゴデータコピー
	memcpy(newdata,olddata,LOGO_DATASIZE(olddata));

	// ロゴ名設定
	lstrcpy(newdata->name,newname);

	// リストボックスを更新
	DeleteItem(owner,list_n);
	InsertItem(owner,list_n,newdata);

	return TRUE;
}

