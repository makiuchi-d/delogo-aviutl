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

#ifdef __cplusplus
}	// End of extern "C"
#endif

#endif
