/*====================================================================
* 	�I�v�V�����_�C�A���O			optdlg.h
*===================================================================*/
#ifndef ___OPTDLG_H
#define ___OPTDLG_H

#include <windows.h>
#include "filter.h"

#ifdef __cplusplus
extern "C" {
#endif

extern FILTER* optfp;
extern HWND    hcb_logo;	// �R���{�{�b�N�X�̃n���h��
extern HWND    hoptdlg;		// �I�v�V�����_�C�A���O�̃n���h��

// �_�C�A���O�v���V�[�W��
extern BOOL CALLBACK OptDlgProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);


//	���X�g�A�C�e���ҏW�p�֐�
extern void InsertItem(HWND hdlg,int n,void *data);
extern void DeleteItem(HWND list,int num);

#ifdef __cplusplus
}	// End of extern "C"
#endif

#endif
