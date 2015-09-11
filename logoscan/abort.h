/*====================================================================
* 	abort.h
*===================================================================*/
#ifndef ___ABORT_H
#define ___ABORT_H

#include <windows.h>
#include "..\filter.h"
#include "scanpix.h"
#include <stdio.h>


// 独自ウィンドウメッセージ
#define WM_SP_DRAWFRAME   WM_USER+50
#define WM_SP_SCANFRAME   WM_USER+51

// 中断ウィンドウ関数
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
	int         mark;
	FILE*       list;
};

#endif
