/*====================================================================
* 	��͌��ʃ_�C�A���O			resultdlg.h
*===================================================================*/
#ifndef ___RESULTDLG_H
#define ___RESULTDLG_H

#include <windows.h>
#include "filter.h"

extern FILTER* dlgfp;	// FILTER�\����
extern char    defname[32];	// �f�t�H���g���S��

// �_�C�A���O�v���V�[�W��
extern BOOL CALLBACK ResultDlgProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
