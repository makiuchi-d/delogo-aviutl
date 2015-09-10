/*====================================================================
* 	abort.h
*===================================================================*/
#ifndef ___ABORT_H
#define ___ABORT_H

#include <windows.h>
#include "..\filter.h"
#include "scanpix.h"


// �Ǝ��E�B���h�E���b�Z�[�W
#define WM_SP_DRAWFRAME   WM_USER+50
#define WM_SP_SCANFRAME   WM_USER+51

// ���f�E�B���h�E�֐�
BOOL CALLBACK AbortDlgProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);


struct AbortDlgParam {
	FILTER*     fp;
	void*       editp;
	ScanPixel*  sp;
	int         s,e;
	int         max_w;
	int         x,y,w,h,t;
	void**      data;
	const char* errstr;
};

#endif
