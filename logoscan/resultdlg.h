/*====================================================================
* 	解析結果ダイアログ			resultdlg.h
* 								(最終更新：2003/05/10)
*===================================================================*/
#ifndef ___RESULTDLG_H
#define ___RESULTDLG_H

#include <windows.h>
#include "filter.h"

extern FILTER* dlgfp;	// FILTER構造体
extern char    defname[32];	// デフォルトロゴ名

// ダイアログプロシージャ
extern BOOL CALLBACK ResultDlgProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
