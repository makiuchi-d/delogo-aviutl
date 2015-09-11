/*====================================================================
* 	�ҏW�_�C�A���O			editdlg.c
*===================================================================*/
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "editdlg.h"
#include "logo.h"
#include "optdlg.h"
#include "logodef.h"


extern char filter_name[];	// �t�B���^��[filter.c]

static HWND owner;	// �e�E�C���h�E
static int  list_n;

//----------------------------
//	�֐��v���g�^�C�v
//----------------------------
void on_wm_initdialog(HWND hdlg);
BOOL on_IDOK(HWND hdlg);

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
					if(on_IDOK(hdlg))
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
	LOGO_HEADER* lp;

	// ���S�f�[�^�擾
	lp = (LOGO_HEADER *)SendDlgItemMessage(owner,IDC_LIST,LB_GETITEMDATA,list_n,0);

	// �G�f�B�b�g�{�b�N�X
	SendDlgItemMessage(hdlg,ID_EDIT_NAME,EM_SETLIMITTEXT,LOGO_MAX_NAME-1,0);
	SendDlgItemMessage(hdlg,ID_EDIT_START, EM_SETLIMITTEXT, 4,0);
	SendDlgItemMessage(hdlg,ID_EDIT_END,   EM_SETLIMITTEXT, 4,0);
	SendDlgItemMessage(hdlg,ID_EDIT_FIN,   EM_SETLIMITTEXT, 3,0);
	SendDlgItemMessage(hdlg,ID_EDIT_FOUT,  EM_SETLIMITTEXT, 3,0);
	SendDlgItemMessage(hdlg,ID_EDIT_SPINST, UDM_SETRANGE, 0, LOGO_STED_MAX);
	SendDlgItemMessage(hdlg,ID_EDIT_SPINED, UDM_SETRANGE, 0, LOGO_STED_MAX);
	SendDlgItemMessage(hdlg,ID_EDIT_SPINFI, UDM_SETRANGE, 0, LOGO_FADE_MAX);
	SendDlgItemMessage(hdlg,ID_EDIT_SPINFO, UDM_SETRANGE, 0, LOGO_FADE_MAX);
	SetDlgItemText(hdlg,ID_EDIT_NAME,lp->name);
	SetDlgItemInt(hdlg,ID_EDIT_START,lp->st,FALSE);
	SetDlgItemInt(hdlg,ID_EDIT_END  ,lp->ed,FALSE);
	SetDlgItemInt(hdlg,ID_EDIT_FIN  ,lp->fi,FALSE);
	SetDlgItemInt(hdlg,ID_EDIT_FOUT ,lp->fo,FALSE);

	// �L���v�V����
	wsprintf(title,"%s - �ҏW",lp->name);
	SetWindowText(hdlg,title);
}

/*--------------------------------------------------------------------
* 	on_IDOK()	OK�{�^������
* 		���S�f�[�^�̕ύX
*-------------------------------------------------------------------*/
BOOL on_IDOK(HWND hdlg)
{
	LOGO_HEADER *newdata;
	LOGO_HEADER *olddata;

	char newname[LOGO_MAX_NAME];
	int  num;

	// �V���S���O
	GetDlgItemText(hdlg,ID_EDIT_NAME,newname,LOGO_MAX_NAME);
	// ���X�g�{�b�N�X������
	num = SendDlgItemMessage(owner,IDC_LIST,LB_FINDSTRING,-1,(WPARAM)newname);
	if(num!=CB_ERR && num!=list_n){	// �ҏW���̂��̈ȊO�ɓ�������������
		MessageBox(hdlg,"�����̃��S������܂�\n�ʂ̖��̂�ݒ肵�Ă�������",filter_name,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	olddata = (LOGO_HEADER *)SendDlgItemMessage(owner,IDC_LIST,LB_GETITEMDATA,list_n,0);

	// �������m��
	newdata = (LOGO_HEADER *)malloc(LOGO_DATASIZE(olddata));
	if(newdata==NULL){
		MessageBox(hdlg,"���������m�ۂł��܂���ł���",filter_name,MB_OK|MB_ICONERROR);
		return TRUE;
	}
	// ���S�f�[�^�R�s�[
	memcpy(newdata,olddata,LOGO_DATASIZE(olddata));

	// ���S�f�[�^�ݒ�
	lstrcpy(newdata->name,newname);
	newdata->st = min(GetDlgItemInt(hdlg,ID_EDIT_START,NULL,FALSE),LOGO_STED_MAX);
	newdata->ed = min(GetDlgItemInt(hdlg,ID_EDIT_END,  NULL,FALSE),LOGO_STED_MAX);
	newdata->fi = min(GetDlgItemInt(hdlg,ID_EDIT_FIN,  NULL,FALSE),LOGO_FADE_MAX);
	newdata->fo = min(GetDlgItemInt(hdlg,ID_EDIT_FOUT, NULL,FALSE),LOGO_FADE_MAX);

	// ���X�g�{�b�N�X���X�V
	DeleteItem(owner,list_n);
	InsertItem(owner,list_n,newdata);

	return TRUE;
}

