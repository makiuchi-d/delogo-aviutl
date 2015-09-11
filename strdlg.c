/*====================================================================
* 	�ҏW�_�C�A���O			editdlg.c
*===================================================================*/
#include <windows.h>
#include "resource.h"
#include "strdlg.h"


//----------------------------
//	�֐��v���g�^�C�v
//----------------------------
static BOOL CopyTextToClipboard(HWND hwnd,const char* text);


/*====================================================================
* 	StrDlgProc()		�R�[���o�b�N�v���V�[�W��
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
* 	CopyTextToClipboard()	�N���b�v�{�[�h�ɃR�s�[
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

	EmptyClipboard();
	SetClipboardData(CF_TEXT,hglbCopy);
	CloseClipboard();

	return TRUE;
}
