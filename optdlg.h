/*====================================================================
* 	オプションダイアログ			optdlg.h
*===================================================================*/
#ifndef ___OPTDLG_H
#define ___OPTDLG_H

#include <windows.h>
#include "filter.h"

#ifdef __cplusplus
extern "C" {
#endif

extern FILTER* optfp;
extern HWND    hcb_logo;	// コンボボックスのハンドル
extern HWND    hoptdlg;		// オプションダイアログのハンドル

// ダイアログプロシージャ
extern BOOL CALLBACK OptDlgProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);


//	リストアイテム編集用関数
extern void InsertItem(HWND hdlg,int n,void *data);
extern void DeleteItem(HWND list,int num);

#ifdef __cplusplus
}	// End of extern "C"
#endif

#endif
