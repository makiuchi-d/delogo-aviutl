/*====================================================================
* 	�ҏW�_�C�A���O			editdlg.c
*===================================================================*/
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "editdlg.h"
#include "logo.h"
#include "optdlg.h"

extern char filter_name[];	// �t�B���^��[filter.c]

static HWND owner;	// �e�E�C���h�E
static int  list_n;

//----------------------------
//	�֐��v���g�^�C�v
//----------------------------
void on_wm_initdialog(HWND hdlg);
void on_IDOK(HWND hdlg);

/*====================================================================
* 	EditDlgProc()		�R�[���o�b�N�v���V�[�W��
*===================================================================*/
BOOL CALLBACK EditDlgProc(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg){
		case WM_INITDIALOG:
			owner = GetWindow(hdlg,GW_OWNER);
			list_n = (int)lParam;
			on_wm_initdialog(hdlg);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDOK:
					on_IDOK(hdlg);
					EndDialog(hdlg,LOWORD(wParam));
					break;

				case IDCANCEL:
					EndDialog(hdlg,LOWORD(wParam));
					break;
			}
	}

	return FALSE;
}


/*--------------------------------------------------------------------
* 	on_wm_initdialog()	������
*-------------------------------------------------------------------*/
void on_wm_initdialog(HWND hdlg)
{
	char title[LOGO_MAX_NAME+10];

	// ���S���擾
	SendDlgItemMessage(owner,IDC_LIST,LB_GETTEXT,list_n,(LPARAM)title);

	// ���S���G�f�B�b�g�{�b�N�X
	SendDlgItemMessage(hdlg,ID_EDIT_NAME,EM_SETLIMITTEXT,LOGO_MAX_NAME-1,0);
	SetDlgItemText(hdlg,ID_EDIT_NAME,title);

	// �L���v�V����
	wsprintf(title,"%s - �ҏW",title);
	SetWindowText(hdlg,title);
}

/*--------------------------------------------------------------------
* 	on_IDOK()	OK�{�^������
* 		���S���̕ύX
*-------------------------------------------------------------------*/
void on_IDOK(HWND hdlg)
{
	LOGO_HEADER *newdata;
	LOGO_HEADER *olddata;

	olddata = (LOGO_HEADER *)SendDlgItemMessage(owner,IDC_LIST,LB_GETITEMDATA,list_n,0);

	// �������m��
	newdata = (LOGO_HEADER *)malloc(LOGO_DATASIZE(olddata));
	if(newdata==NULL){
		MessageBox(hdlg,"������������܂���",filter_name,MB_OK|MB_ICONERROR);
		return;
	}
	// ���S�f�[�^�R�s�[
	memcpy(newdata,olddata,LOGO_DATASIZE(olddata));

	// ���S���ύX
	GetDlgItemText(hdlg,ID_EDIT_NAME,newdata->name,LOGO_MAX_NAME);

	// ���X�g�{�b�N�X���X�V
	DeleteItem(owner,list_n);
	InsertItem(owner,list_n,newdata);
}

