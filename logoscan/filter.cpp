/*********************************************************************
* 	���S��̓v���O�C��		ver 0.07
* 
* 2003
* 	04/06:	�Ƃ肠���������B
* 	04/07:	�o�b�t�@�I�[�o�[�t���[���
* 	04/09:	new�Ƃ�malloc�����Ƃ���ɗ�����̂͂Ȃ��H
* 	04/10:	new[]�ɓn���p�����[�^���������������ׂ������B�C��
* 	04/14:	�w�i�F�v�Z�֐��̍��v���鎞�̃o�O���C���B
* 			���f�B�A�������Ă��畽�ς����悤�ɂ����B
* 			��A�����̎擾�A���S���Y���������ύX�B
* 	04/27:	���S�͈͍ő�l�̕ύX�i��������ɂP���]�T�����������j
* 	04/28:	��͌��ʃ_�C�A���O�\������AviUtl���I���ł��Ȃ��悤�ɕύX
* 			�i�G���[���o���ė�����o�O����j
* 
* [���Ō��J]
* 
* 	05/10:	���S�͈͂̍ő�l���S�{�ɂ����B(logo.h)
* 			�w�i�l�v�Z��ύX�i�\�[�g���Đ^����ւ񂾂����ρj
* 			��͂�255�t���[���𒴂���Ɨ�����o�O�C��		(��02)
* 	06/16:	�G���[���b�Z�[�W���ꕔ�C��
* 			���S�͈͂̌`�ɐF���ςɂȂ�o�O�C��
* 			w��h���ԈႦ�Ă����Ȃ�āc�T��
* 			�w�i���P��F���ǂ���������Ŕ��肷��悤�ɂ���
* 			�I��͈͓���������͂���悤�ɂ���
* 	06/17:	����̏C���œ���Ă��܂����o�O���C��	(��03)
* 	06/18:	�ŏ��̃t���[����\�������Ă���Ɖ�͂ł��Ȃ��o�O�C�� (��03a)
* 			���ʃ_�C�A���O�̃v���r���[�w�i�F��RGB�Ŏw�肷��悤�ɕύX
* 
* [������]
* 	07/02:	���f�ł���悤�ɂ����B
* 			���̂��߂ɏ����̗����啝�ɕύX�B
* 	07/03:	�v���t�@�C���̕ύX�t���[���Ŗ������[�v�ɂȂ�o�O����i0.04)
* 	08/02:	���������̌��������č�����
* 			�ׂ��ȏC���A��͊������Ƀr�[�v��炷�悤�ɂ����B
* 	09/22:	�L���b�V���̕��ƍ������W�̔{���ɂ����B(SSE2�΍�ɂȂ�������?)
* 	09/27:	filter.h��AviUtl0.99SDK�̂��̂ɍ����ւ��B(0.05)
* 	10/14:	�L���b�V��������������ɖ߂����B
* 	10/18:	�L���t���[�����}�[�N����O�t�@�C���o�͂ł���悤�ɂ����B
* 			VirtualAlloc����߂�malloc���g���悤�ɂ����B(0.06)
* 	10/20:	VirtualAlloc�ɂ��ǂ����B
* 			exfunc->rgb2yc����߂āA���O��RGB->YCbCr
* 			�L���t���[�����X�g��ۑ��̃`�F�b�N�������Ă��Ȃ��������Ȃ��o�O�C��
* 			���O�t�@�C���̃f�t�H���g�����\�[�X�t�@�C�����������悤�ɂ����B(0.06a)
* 	10/23:	�L���t���[�����X�g�ۑ��_�C�A���O�ŃL�����Z������Ɨ�����o�O�C���B
* 			���ȋL�q���������̂��C���B�i���œ����Ă��񂾂�c
* 2008
* 	01/07:	���S�T�C�Y�����P�p
* 			���S�t�@�C���̃f�[�^���g���ɔ����C��
* 
*********************************************************************/
/*	TODO:
* 	�E�g��c�[���@�\�i�C�܂���o��������̃A�C�f�B�A)
* 	�E�Z�[�u���͉������Ȃ��悤�ɂ���
* 	�E���ʃ_�C�A���O�ŊJ�n��I����t�F�[�h���������߂�悤�ɂ���
* 
* 	MEMO:
* 	�E�w�i�l�v�Z���P��@�F���f�B�A�������Ă��畽�ςƂ�
* 	�E�w�i�l�v�Z���P��A�F�\�[�g���Đ^����ւ񂾂��Ōv�Z�Ƃ�
* 
* 	�E�w�i���P�F���ǂ����̔���F�w�i�l�̕��ςƁA�ő�or�ŏ��Ƃ̍���臒l�ȏ�̂Ƃ��P��łȂ��Ƃ���̂͂ǂ���
* 		���ő�ƍŏ��̍���臒l�ȏ�̂Ƃ��P��łȂ��Ɣ��f�̂ق����悳�����B
* 
* 	�ESSE2�������ɗ�����Fget_ycp_filtering_cache_ex������ǂ��B�Ƃ肠�������������W�̔{���ɁB
* 		�����߂ہBVirtualAlloc���Ȃ��B�Ƃ肠���������Ă݂�B
* 		AviUtl�{�Ƃ̌f����rgb2yc�������Ȃ��Ƃ̕񍐂�?!�����������̂��B
* 			�����O�ŕϊ��B
* 
*/
#include <windows.h>
#include <stdlib.h>
#include "..\\filter.h"
#include "..\\logo.h"
#include "scanpix.h"
#include "resultdlg.h"
#include "abort.h"


// �{�^��
#define ID_SCANBTN  40010
HWND scanbtn;

static short dn_x,dn_y;	// �}�E�X�_�E�����W
static short up_x,up_y;	// �A�b�v���W
static bool  flg_mouse_down = 0;	// �}�E�X�_�E���t���O
static short _x,_y,_w,_h,_thy;

void *logodata = NULL;	// ���S�f�[�^�i��͌��ʁj

#define LIST_FILTER  "�t���[�����X�g (*.txt)\0*.txt\0"\
                     "�S�Ẵt�@�C�� (*.*)\0*.*\0"

//----------------------------
//	�v���g�^�C�v�錾
//----------------------------
inline void create_dlgitem(HWND hwnd,HINSTANCE hinst);
inline void SetXYWH(FILTER* fp,void* editp);
inline void SetRange(FILTER* fp,void* editp);
inline void FixXYWH(FILTER* fp,void* editp);
void ScanLogoData(FILTER* fp,void* editp);
void SetScanPixel(FILTER*,ScanPixel*&,int,int,int,int,void*,char*);

//----------------------------
//	FILTER_DLL�\����
//----------------------------
char filter_name[] = "���S���";
char filter_info[] = "���S��̓v���O�C�� ver 0.07 by MakKi";

#define track_N 5
#if track_N
TCHAR *track_name[]   = { "�ʒuX","�ʒuY","��","����","臒l" };	// �g���b�N�o�[�̖��O
int   track_default[] = { 1, 1, 1, 1,  30 };	// �g���b�N�o�[�̏����l
int   track_s[]       = { 1, 1, 1, 1,   0 };	// �g���b�N�o�[�̉����l
int   track_e[]       = { 1, 1, 1, 1, 255 };	// �g���b�N�o�[�̏���l
#endif

#define check_N 2
#if check_N
TCHAR *check_name[]   = { "�L���t���[�����}�[�N", "�L���t���[�����X�g��ۑ�" };	// �`�F�b�N�{�b�N�X
int   check_default[] = { 0,0 };	// �f�t�H���g
#endif

#define tLOGOX   0
#define tLOGOY   1
#define tLOGOW   2
#define tLOGOH   3
#define tTHY     4
#define cMARK    0
#define cLIST    1

// �ݒ�E�B���h�E�̍���
#define WND_Y (60+24*track_N+20*check_N)


FILTER_DLL filter = {
	FILTER_FLAG_WINDOW_SIZE |	// �ݒ�E�B���h�E�̃T�C�Y���w��o����悤�ɂ��܂�
	FILTER_FLAG_MAIN_MESSAGE |	// func_WndProc()��WM_FILTER_MAIN_???�̃��b�Z�[�W�𑗂�悤�ɂ��܂�
	FILTER_FLAG_EX_INFORMATION,
#ifdef WND_Y
	320,WND_Y,			// �ݒ�E�C���h�E�̃T�C�Y
#else
	NULL,NULL,
#endif
	filter_name,		// �t�B���^�̖��O
	track_N,        	// �g���b�N�o�[�̐�
#if track_N
	track_name,     	// �g���b�N�o�[�̖��O�S
	track_default,  	// �g���b�N�o�[�̏����l�S
	track_s,track_e,	// �g���b�N�o�[�̐��l�̉������
#else
	NULL,NULL,NULL,NULL,
#endif
	check_N,      	// �`�F�b�N�{�b�N�X�̐�
#if check_N
	check_name,   	// �`�F�b�N�{�b�N�X�̖��O�S
	check_default,	// �`�F�b�N�{�b�N�X�̏����l�S
#else
	NULL,NULL,
#endif
	func_proc,   	// �t�B���^�����֐�
	NULL,NULL,   	// �J�n��,�I�����ɌĂ΂��֐�
	NULL,        	// �ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐�
	func_WndProc,	// �ݒ�E�B���h�E�v���V�[�W��
	NULL,NULL,   	// �V�X�e���Ŏg�p
	NULL,NULL,     	// �g���f�[�^�̈�
	filter_info,	// �t�B���^���
	NULL,			// �Z�[�u�J�n���O�ɌĂ΂��֐�
	NULL,			// �Z�[�u�I�����ɌĂ΂��֐�
	NULL,NULL,NULL,	// �V�X�e���Ŏg�p
	NULL,			// �g���̈揉���l
};

/*********************************************************************
*	DLL Export
*********************************************************************/
EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable( void )
{
	return &filter;
}

/*====================================================================
*	�t�B���^�����֐�
*===================================================================*/
BOOL func_proc(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	// �ҏW���ȊO�͉������Ȃ�
	if(!fp->exfunc->is_editing(fpip->editp))
		return FALSE;

	// �͈͊O
	if(fp->track[tLOGOX]==0 || fp->track[tLOGOY]==0) return FALSE;
	if(fp->track[tLOGOX]+fp->track[tLOGOW] > fpip->w) return FALSE;
	if(fp->track[tLOGOY]+fp->track[tLOGOH] > fpip->h) return FALSE;

	PIXEL_YC* ptr;
	int i;

	// �g����������(�T�s�N�Z���O���Ɂj
	// X-1,Y-1�Ɉړ�
	ptr = fpip->ycp_edit + (fp->track[tLOGOX]-1) + (fp->track[tLOGOY]-1) * fpip->max_w;
	// �����i��j�l�K�|�W
	for(i=0;i<=fp->track[tLOGOW]+1;i++){
		ptr->y = 4096 - ptr->y;
		ptr->cb *= -1;
		ptr->cr *= -1;
		ptr++;
	}
	ptr += fpip->max_w - i;
	// �c��
	for(i=1;i<=fp->track[tLOGOH];i++){
		// ����
		ptr->y = 4096 - ptr->y;
		ptr->cb *= -1;
		ptr->cr *= -1;
		// �E��
		if(fp->track[tLOGOW]>=0){
			ptr[fp->track[tLOGOW]+1].y  = 4096 - ptr[fp->track[tLOGOW]+1].y;
			ptr[fp->track[tLOGOW]+1].cb *= -1;
			ptr[fp->track[tLOGOW]+1].cr *= -1;
		}
		ptr += fpip->max_w;
	}
	// �����i���j
	if(fp->track[tLOGOH]>=0){
		for(i=0;i<=fp->track[tLOGOW]+1;i++){
			ptr->y = 4096 - ptr->y;
			ptr->cb *= -1;
			ptr->cr *= -1;
			ptr++;
		}
	}

	return TRUE;
}

/*====================================================================
*	�ݒ�E�B���h�E�v���V�[�W��
*===================================================================*/
BOOL func_WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp )
{
	static bool scanning;

	switch(message){
		case WM_FILTER_INIT:	// ������
			create_dlgitem(hwnd,fp->dll_hinst);
			scanning = false;
			break;

		case WM_FILTER_CHANGE_PARAM:
			if(scanning){
				FixXYWH(fp,editp);
				return TRUE;
			}
			SetRange(fp,editp);
			return FALSE;

		//--------------------------------------------�}�E�X���b�Z�[�W
		case WM_FILTER_MAIN_MOUSE_DOWN:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;	// �����̎��������Ȃ�
			dn_x = up_x = (short)LOWORD(lparam);
			dn_y = up_y = (short)HIWORD(lparam);
			flg_mouse_down = true;
			if(!scanning) SetXYWH(fp,editp);
			return TRUE;

		case WM_FILTER_MAIN_MOUSE_UP:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;
			if(flg_mouse_down){	// �}�E�X��������Ă��鎞
				up_x = (short)LOWORD(lparam);
				up_y = (short)HIWORD(lparam);
				flg_mouse_down = false;
				if(!scanning) SetXYWH(fp,editp);
				return TRUE;
			}
			break;

		case WM_FILTER_MAIN_MOUSE_MOVE:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;
			if(flg_mouse_down){	// �}�E�X��������Ă��鎞
				up_x = (short)LOWORD(lparam);
				up_y = (short)HIWORD(lparam);
				if(!scanning) SetXYWH(fp,editp);
				return TRUE;
			}
			break;

		//----------------------------------------------���S��̓{�^��
		case WM_COMMAND:
			switch(LOWORD(wparam)){
				case ID_SCANBTN:
					scanning = true;
					ScanLogoData(fp,editp);
					scanning = false;
					break;
			}
			break;

		case WM_KEYUP:	// ���C���E�B���h�E�֑���
		case WM_KEYDOWN:
		case WM_MOUSEWHEEL:
			SendMessage(GetWindow(hwnd, GW_OWNER), message, wparam, lparam);
			break;

		//----------------------------------------------�Ǝ����b�Z�[�W
		case WM_SP_DRAWFRAME:
			fp->exfunc->set_frame(editp,lparam);
			return TRUE;
	}

	return FALSE;
}

/*--------------------------------------------------------------------
*	�_�C�A���O�A�C�e�������
*-------------------------------------------------------------------*/
inline void create_dlgitem(HWND hwnd,HINSTANCE hinst)
{
#define ITEM_Y (14+24*track_N+20*check_N)
	HFONT font;
	font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// ���S��̓{�^��
	scanbtn = CreateWindow("BUTTON", "���S���", WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON|BS_VCENTER,
									10,ITEM_Y, 295,18, hwnd, (HMENU)ID_SCANBTN, hinst, NULL);
	SendMessage(scanbtn, WM_SETFONT, (WPARAM)font, 0);
}

/*--------------------------------------------------------------------
*	�ݒ�E�B���h�E�̊e�l���Œ肷��
*-------------------------------------------------------------------*/
inline void FixXYWH(FILTER* fp,void* editp)
{
	fp->track[tLOGOX] = _x;
	fp->track[tLOGOY] = _y;
	fp->track[tLOGOW] = _w;
	fp->track[tLOGOH] = _h;
	fp->track[tTHY]   = _thy;

	fp->exfunc->filter_window_update(fp);	// �X�V
}

/*--------------------------------------------------------------------
*	�ݒ�E�B���h�E�̊e�l��ݒ肷��
*-------------------------------------------------------------------*/
inline void SetXYWH(FILTER* fp,void* editp)
{
	int h,w;

	if(!fp->exfunc->get_frame_size(editp,&w,&h))
		// �擾���s
		return;

	// �摜���Ɏ��߂�
	if(dn_x<0) dn_x = 0;
	else if(dn_x>=w) dn_x = w -1;
	if(dn_y<0) dn_y = 0;
	else if(dn_y>=h) dn_y = h -1;
	if(up_x<0) up_x = 0;
	else if(up_x>=w) up_x = w -1;
	if(up_y<0) up_y = 0;
	else if(up_y>=h) up_y = h -1;


	// �ݒ�E�B���h�̊e�l��ݒ�
	fp->track_e[tLOGOX] = w;	// X�ő�l
	fp->track_e[tLOGOY] = h;	// Y�ő�l
	fp->track[tLOGOX]   = ((dn_x<up_x)?dn_x:up_x) + 1;
	fp->track[tLOGOY]   = ((dn_y<up_y)?dn_y:up_y) + 1;
	fp->track_e[tLOGOW] = w - fp->track[tLOGOX];
	fp->track_e[tLOGOH] = h - fp->track[tLOGOY];
	fp->track[tLOGOW]   = ((dn_x<up_x)?up_x:dn_x) - fp->track[tLOGOX];
	fp->track[tLOGOH]   = ((dn_y<up_y)?up_y:dn_y) - fp->track[tLOGOY];

	_x = fp->track[tLOGOX]; _y = fp->track[tLOGOY];
	_w = fp->track[tLOGOW]; _h = fp->track[tLOGOH];
	_thy = fp->track[tTHY];

	fp->exfunc->filter_window_update(fp);	// �X�V
}

/*--------------------------------------------------------------------
*	�g���b�N�o�[�̍ő�l��ݒ肷��
*-------------------------------------------------------------------*/
inline void SetRange(FILTER* fp,void* editp)
{
	int h,w;

	if(!fp->exfunc->get_frame_size(editp,&w,&h))
		// �擾���s
		return;

	fp->track_e[tLOGOX] = w;	// X�ő�l
	fp->track_e[tLOGOY] = h;	// Y�ő�l
	fp->track_e[tLOGOW] = w - fp->track[tLOGOX] -1;	// ���ő�l
	fp->track_e[tLOGOH] = h - fp->track[tLOGOY] -1;	// �����ő�l

	if(fp->track_e[tLOGOX] < fp->track[tLOGOX])
		fp->track[tLOGOX] = fp->track_e[tLOGOX];	// �ő�l�ɂ��킹��
	if(fp->track_e[tLOGOY] < fp->track[tLOGOY])
		fp->track[tLOGOY] = fp->track_e[tLOGOY];
	if(fp->track_e[tLOGOW] < fp->track[tLOGOW])
		fp->track[tLOGOW] = fp->track_e[tLOGOW];
	if(fp->track_e[tLOGOH] < fp->track[tLOGOH])
		fp->track[tLOGOH] = fp->track_e[tLOGOH];

	_x = fp->track[tLOGOX]; _y = fp->track[tLOGOY];
	_w = fp->track[tLOGOW]; _h = fp->track[tLOGOH];
	_thy = fp->track[tTHY];

	fp->exfunc->filter_window_update(fp);	// �X�V
}

/*--------------------------------------------------------------------
*	���S�f�[�^����͂���
*-------------------------------------------------------------------*/
void ScanLogoData(FILTER* fp,void* editp)
{
	EnableWindow(scanbtn,FALSE);	// ��̓{�^��������

	int w,h;		// ��,����
	int start,end;	// �I���J�n�E�I���t���[��
	int frame;		// ���݂̕\���t���[��
	char list[MAX_PATH] = "\0";	// �t���[�����X�g�t�@�C����

	ScanPixel*  sp = NULL;
	LOGO_HEADER lgh;

	try{
		if(fp->exfunc->is_filter_active(fp)==FALSE)	// �t�B���^���L���łȂ���
			throw "�t�B���^��L���ɂ��Ă�������";

		// �K�v�ȏ����W�߂�
		frame   = fp->exfunc->get_frame_n(editp);
		if(!frame) throw "�f�����ǂݍ��܂�Ă��܂���";

		frame   = fp->exfunc->get_frame(editp);

		fp->exfunc->get_select_frame(editp,&start,&end);
		if(end-start<1) throw "�摜�̖���������܂���";

//		if((fp->track[tLOGOW]+1)*(fp->track[tLOGOH]+1) > LOGO_MAXPIXEL)
//			// h*w�����S�f�[�^������傫����
//			throw "���S�̈悪�L�����܂�";

		// �摜�T�C�Y
		if(!fp->exfunc->get_frame_size(editp,&w,&h))
			throw "�摜�T�C�Y�擾�ł��܂���ł���";

		// ���S���̏����l
		GetWindowText(GetWindow(fp->hwnd,GW_OWNER),defname,LOGO_MAX_NAME-9);	// �^�C�g���o�[������擾
		for(int i=1;i<LOGO_MAX_NAME-9;i++)
			if(defname[i]=='.') defname[i] = '\0';	// 2�����ڈȍ~��'.'���I�[�ɂ���i.avi���폜�j
		wsprintf(defname,"%s %dx%d",defname,w,h);	// �f�t�H���g���S���쐬

		// �L���b�V���T�C�Y�ݒ�
		fp->exfunc->set_ycp_filtering_cache_size(fp,w,h,1,NULL);

		if(fp->check[cLIST]){	// ���X�g�ۑ����t�@�C�����擾
			// ���S���̏����l
			GetWindowText(GetWindow(fp->hwnd,GW_OWNER),list,MAX_PATH-10);	// �^�C�g���o�[������擾
			for(int i=1;list[i]&&i<MAX_PATH-10;i++)
				if(list[i]=='.') list[i] = '\0';	// 2�����ڈȍ~��'.'���I�[�ɂ���i�g���q���폜�j
			wsprintf(list,"%s_scan.txt",list);	// �f�t�H���g���S���쐬

			if(!fp->exfunc->dlg_get_save_name(list,LIST_FILTER,list))
				list[0] = '\0';	// �L�����Z����
		}

		// ScanPixel��ݒ肷��+��́E���S�f�[�^�쐬
		SetScanPixel(fp,sp,w,h,start,end,editp,list);
	}
	catch(const char* str){
		MessageBox(fp->hwnd,str,filter_name,MB_OK|MB_ICONERROR);
		if(sp) delete[] sp;
		sp=NULL;
		if(logodata) delete[] logodata;
		logodata=NULL;
		EnableWindow(scanbtn,TRUE);	// �{�^����L���ɖ߂�

		return;
	}

	if(sp){
		delete[] sp;
		sp=NULL;
	}


	// �\���t���[����߂�
	fp->exfunc->set_frame(editp,frame);

	// ��͌��ʃ_�C�A���O
	dlgfp = fp;
	DialogBox(fp->dll_hinst,"RESULT_DLG",GetWindow(fp->hwnd,GW_OWNER),ResultDlgProc);

	if(logodata){
		delete[] logodata;
		logodata=NULL;
	}

	EnableWindow(scanbtn,TRUE);	// �{�^����L���ɖ߂�
}

/*--------------------------------------------------------------------
*	ScanPixel��ݒ肷��
*-------------------------------------------------------------------*/
void SetScanPixel(FILTER* fp,ScanPixel*& sp,int w,int h,int s,int e,void* editp,char* list)
{
	// �͈̓`�F�b�N
	if(fp->track[tLOGOW]<=0 || fp->track[tLOGOH]<=0)
		throw "�̈悪�w�肳��Ă��܂���";
	if( (fp->track[tLOGOX]+fp->track[tLOGOW] > w-1) ||
		(fp->track[tLOGOY]+fp->track[tLOGOH] > h-1) )
			throw "�̈�̈ꕔ����ʊO�ł�";

	// �������m��
	if(sp) delete[] sp;
	ScanPixel::Defbuf = 1024;
	sp = new ScanPixel[fp->track[tLOGOW]*fp->track[tLOGOH]];	// ���~�����̔z��
	if(sp==NULL)
		throw "���������m�ۂł��܂���ł���";

	AbortDlgParam param;

	param.fp     = fp;
	param.editp  = editp;
	param.sp     = sp;
	param.s      = s;
	param.e      = e;
	param.max_w  = w;
	param.x      = fp->track[tLOGOX];
	param.y      = fp->track[tLOGOY];
	param.w      = fp->track[tLOGOW];
	param.h      = fp->track[tLOGOH];
	param.t      = fp->track[tTHY];
	param.data   = &logodata;
	param.errstr = NULL;
	param.mark   = fp->check[cMARK];
	param.list   = NULL;

	if(*list){
		param.list = fopen(list,"w");
		if(param.list==NULL){
			throw "�t���[�����X�g�t�@�C���̍쐬�Ɏ��s���܂���";
		}
		fprintf(param.list,"<Frame List>\n");
	}

	DialogBoxParam(fp->dll_hinst,"ABORT_DLG",GetWindow(fp->hwnd,GW_OWNER),AbortDlgProc,(LPARAM)&param);

	if(param.list) fclose(param.list);
	param.list = NULL;

	if(param.errstr)
		throw param.errstr;
}



//*/
