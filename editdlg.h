/*====================================================================
* 	編集ダイアログ			editdlg.h
*===================================================================*/
#ifndef ___EDITDLG_H
#define ___EDITDLG_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// ダイアログプロシージャ
extern BOOL CALLBACK EditDlgProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef __cplusplus
}	// End of extern "C"
#endif

#endif
