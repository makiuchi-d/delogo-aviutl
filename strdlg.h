/*====================================================================
* 	������\���_�C�A���O			strdlg.h
*===================================================================*/
#ifndef ___STRDLG_H
#define ___STRDLG_H

#include <windows.h>

#define STRDLG_MAXSTR MAX_PATH+256

#ifdef __cplusplus
extern "C" {
#endif

// �_�C�A���O�v���V�[�W��
extern BOOL CALLBACK StrDlgProc(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam);

#ifdef __cplusplus
}	// End of extern "C"
#endif

#endif
