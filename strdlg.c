/*====================================================================
* 	編集ダイアログ			editdlg.c
*===================================================================*/
#include <windows.h>
#include "resource.h"
#include "strdlg.h"


//----------------------------
//	関数プロトタイプ
//----------------------------
static BOOL CopyTextToClipboard(HWND hwnd,const char* text);


/*====================================================================
* 	StrDlgProc()		コールバックプロシージャ
*===================================================================*/
BOOL CALLBACK StrDlgProc(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static char str[STRDLG_MAXSTR];

	switch(msg){
		case WM_INITDIALOG:
			SetDlgItemText(hdlg,ID_SHOW_STRING,(const char*)lParam);
			lstrcpy(str,(const char*)lParam);
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDOK:
				case IDCANCEL:
					EndDialog(hdlg,LOWORD(wParam));
					return TRUE;

				case ID_COPY_STRING:
					 CopyTextToClipboard(hdlg,str);
			}
			break;
	}

	return FALSE;
}

/*--------------------------------------------------------------------
* 	CopyTextToClipboard()	クリップボードにコピー
*-------------------------------------------------------------------*/
static BOOL CopyTextToClipboard(HWND hwnd,const char* text)
{
	HGLOBAL hglbCopy;
	char*   ptrCopy;

	if(!OpenClipboard(hwnd)) return FALSE;

	hglbCopy = GlobalAlloc(GMEM_MOVEABLE,lstrlen(text)+1);
	if(hglbCopy==NULL){
		CloseClipboard();
		return FALSE;
	}

	ptrCopy = (char*)GlobalLock(hglbCopy);
	lstrcpy(ptrCopy,text);
	GlobalUnlock(hglbCopy);

	if(!EmptyClipboard()){
		GlobalFree(hglbCopy);
		CloseClipboard();
		return FALSE;
	}
	if(SetClipboardData(CF_TEXT,hglbCopy)==NULL){
		GlobalFree(hglbCopy);
		CloseClipboard();
		return FALSE;
	}
	CloseClipboard();

	return TRUE;
}
