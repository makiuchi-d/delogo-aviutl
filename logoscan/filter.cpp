/*********************************************************************
* 	���S��̓v���O�C��		ver.��02
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
* 			�w�i���P��F���ǂ���������Ŕ��肷��悤�ɂ����B
* 	06/17:	����̏C���œ���Ă��܂����o�O���C��
* 
*********************************************************************/
/*	TODO:
* 	�E���f�����t���Ȃ��Ƃȁ[
* 
* 	MEMO:
* 	�E�w�i�l�v�Z���P��@�F���f�B�A�������Ă��畽�ςƂ�
* 	�E�w�i�l�v�Z���P��A�F�\�[�g���Đ^����ւ񂾂��Ōv�Z�Ƃ�
* 
* 	�E�w�i���P�F���ǂ����̔���F�w�i�l�̕��ςƁA�ő�or�ŏ��Ƃ̍���臒l�ȏ�̂Ƃ��P��łȂ��Ƃ���̂͂ǂ���
* 		���ő�ƍŏ��̍���臒l�ȏ�̂Ƃ��P��łȂ��Ɣ��f�̂ق����悳�����B
* 
*/
#include <windows.h>
#include <stdlib.h>
#include "..\\filter.h"
#include "..\\logo.h"
#include "scanpix.h"
#include "resultdlg.h"

//#include <stdio.h>


// �{�^��
#define ID_SCANBTN  40010
HWND scanbtn;

short dn_x,dn_y;	// �}�E�X�_�E�����W
short up_x,up_y;	// �A�b�v���W
bool  flg_mouse_down = 0;	// �}�E�X�_�E���t���O

void *logodata = NULL;	// ���S�f�[�^�i��͌��ʁj

class XYWH {
public:
	short x;
	short y;
	short w;
	short h;

	XYWH(void)
		{ x=y=w=h=-1; }
	XYWH(XYWH& r)
		{ x=r.x; y=r.y; w=r.w; h=r.h; }
	XYWH(int nx,int ny,int nw,int nh)
		{ x=nx; y=ny; w=nw; h=nh; }
};
//----------------------------
//	�v���g�^�C�v�錾
//----------------------------
inline void create_dlgitem(HWND hwnd,HINSTANCE hinst);
inline void SetXYWH(FILTER* fp,void* editp);
inline void SetRange(FILTER* fp,void* editp);
void ScanLogoData(FILTER* fp,void* editp);
void SetScanPixel(FILTER*,ScanPixel*&,int,int,int,int,void*);
bool Cal_BGcolor(PIXEL_YC&,PIXEL_YC*,XYWH&,int,int);
void CreateLogoData(FILTER* fp,ScanPixel*& sp);
int  comp_short(const void* x,const void* y);
static short med_average(short* s,int n);

//----------------------------
//	FILTER_DLL�\����
//----------------------------
char filter_name[] = "���S���";
char filter_info[] = "���S��̓v���O�C�� ver.��03 by MakKi";

#define track_N 5
#if track_N
TCHAR *track_name[]   = { "�ʒuX","�ʒuY","��","����","臒l" };	// �g���b�N�o�[�̖��O
int   track_default[] = { 1, 1, 1, 1,  30 };	// �g���b�N�o�[�̏����l
int   track_s[]       = { 1, 1, 1, 1,   0 };	// �g���b�N�o�[�̉����l
int   track_e[]       = { 1, 1, 1, 1, 255 };	// �g���b�N�o�[�̏���l
#endif

#define check_N 0
#if check_N
TCHAR *check_name[]   = { 0 };	// �`�F�b�N�{�b�N�X
int   check_default[] = { 0 };	// �f�t�H���g
#endif

#define tLOGOX   0
#define tLOGOY   1
#define tLOGOW   2
#define tLOGOH   3
#define tTHY     4

// �ݒ�E�B���h�E�̍���
#define WND_Y (60+24*track_N+20*check_N)


FILTER_DLL filter = {
	FILTER_FLAG_WINDOW_SIZE |			// �ݒ�E�B���h�E�̃T�C�Y���w��o����悤�ɂ��܂�
	FILTER_FLAG_MAIN_MESSAGE | 	// func_WndProc()��WM_FILTER_MAIN_???�̃��b�Z�[�W�𑗂�悤�ɂ��܂�
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

	XYWH logo(fp->track[tLOGOX],fp->track[tLOGOY],fp->track[tLOGOW],fp->track[tLOGOH]);

	// �͈͊O
	if(logo.x==0 || logo.y==0) return FALSE;
	if(logo.x+logo.w > fpip->w) return FALSE;
	if(logo.y+logo.h > fpip->h) return FALSE;

	PIXEL_YC* ptr;
	int i;

	// �g����������(�T�s�N�Z���O���Ɂj
	// X-1,Y-1�Ɉړ�
	ptr = fpip->ycp_edit + (logo.x-1) + (logo.y-1) * fpip->max_w;
	// �����i��j�l�K�|�W
	for(i=0;i<=logo.w+1;i++){
		ptr->y = 4096 - ptr->y;
		ptr->cb *= -1;
		ptr->cr *= -1;
		ptr++;
	}
	ptr += fpip->max_w - i;
	// �c��
	for(i=1;i<=logo.h;i++){
		// ����
		ptr->y = 4096 - ptr->y;
		ptr->cb *= -1;
		ptr->cr *= -1;
		// �E��
		if(logo.w>=0){
			ptr[logo.w+1].y  = 4096 - ptr[logo.w+1].y;
			ptr[logo.w+1].cb *= -1;
			ptr[logo.w+1].cr *= -1;
		}
		ptr += fpip->max_w;
	}
	// �����i���j
	if(logo.h>=0){
		for(i=0;i<=logo.w+1;i++){
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
	switch(message){
		case WM_FILTER_INIT:	// ������
			create_dlgitem(hwnd,fp->dll_hinst);
			break;

		case WM_FILTER_CHANGE_PARAM:
			SetRange(fp,editp);
			return TRUE;

		//--------------------------------------------�}�E�X���b�Z�[�W
		case WM_FILTER_MAIN_MOUSE_DOWN:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;	// �����̎��������Ȃ�
			dn_x = up_x = (short)LOWORD(lparam);
			dn_y = up_y = (short)HIWORD(lparam);
			flg_mouse_down = true;
			SetXYWH(fp,editp);
			return TRUE;

		case WM_FILTER_MAIN_MOUSE_UP:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;
			if(flg_mouse_down){	// �}�E�X��������Ă��鎞
				up_x = (short)LOWORD(lparam);
				up_y = (short)HIWORD(lparam);
				flg_mouse_down = false;
				SetXYWH(fp,editp);
				return TRUE;
			}
			break;

		case WM_FILTER_MAIN_MOUSE_MOVE:
			if(!fp->exfunc->is_filter_active(fp))
				return FALSE;
			if(flg_mouse_down){	// �}�E�X��������Ă��鎞
				up_x = (short)LOWORD(lparam);
				up_y = (short)HIWORD(lparam);
				SetXYWH(fp,editp);
				return TRUE;
			}
			break;

		//----------------------------------------------���S��̓{�^��
		case WM_COMMAND:
			switch(LOWORD(wparam)){
				case ID_SCANBTN:
					ScanLogoData(fp,editp);
					return TRUE;
			}
			break;

		case WM_KEYUP:	// ���C���E�B���h�E�֑���
		case WM_KEYDOWN:
		case WM_MOUSEWHEEL:
			SendMessage(GetWindow(hwnd, GW_OWNER), message, wparam, lparam);
			break;
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

	ScanPixel*  sp = NULL;
	LOGO_HEADER lgh;

	try{
		if(fp->exfunc->is_filter_active(fp)==FALSE)	// �t�B���^���L���łȂ���
			throw "�t�B���^��L���ɂ��Ă�������";

		// �K�v�ȏ����W�߂�
		frame   = fp->exfunc->get_frame(editp);
		if(!frame) throw "�f�����ǂݍ��܂�Ă��܂���";
		fp->exfunc->get_select_frame(editp,&start,&end);
		if(end-start<1) throw "�摜�̖���������܂���";

		if((fp->track[tLOGOW]+1)*(fp->track[tLOGOH]+1) > LOGO_MAXPIXEL)
			// h*w�����S�f�[�^������傫����
			throw "���S�̈悪�L�����܂�";

		// �摜�T�C�Y
		if(!fp->exfunc->get_frame_size(editp,&w,&h))
			throw "�摜�T�C�Y�擾�ł��܂���ł���";

		// �L���b�V���T�C�Y�ݒ�
		fp->exfunc->set_ycp_filtering_cache_size(fp,w,h,1,NULL);

		// ���S���̏����l
		GetWindowText(GetWindow(fp->hwnd,GW_OWNER),defname,LOGO_MAX_NAME-9);	// �^�C�g���o�[������擾
		for(int i=1;i<LOGO_MAX_NAME-9;i++)
			if(defname[i]=='.') defname[i] = '\0';	// 2�����ڈȍ~��'.'���I�[�ɂ���
		wsprintf(defname,"%s %dx%d",defname,w,h);	// �f�t�H���g���S���쐬

		// ScanPixel��ݒ肷��
		SetScanPixel(fp,sp,w,h,start,end,editp);

		// ��́E���S�f�[�^�쐬
		CreateLogoData(fp,sp);
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

	SetWindowText(GetWindow(fp->hwnd,GW_OWNER),"��͊���");

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
void SetScanPixel(FILTER* fp,ScanPixel*& sp,int w,int h,int s,int e,void* editp)
{
	XYWH xywh(fp->track[tLOGOX],fp->track[tLOGOY],fp->track[tLOGOW],fp->track[tLOGOH]);

	// �͈̓`�F�b�N
	if(xywh.w<=0 || xywh.h<=0)
		throw "�̈悪�w�肳��Ă��܂���";
	if( (xywh.x+xywh.w > w-1) ||
		(xywh.y+xywh.h > h-1) )
			throw "�̈�̈ꕔ����ʊO�ł�";

	// �������m��
	if(sp) delete[] sp;
	sp = new ScanPixel[xywh.w*xywh.h];	// ���~�����̔z��
	if(sp==NULL)
		throw "���������m�ۂł��܂���ł���";

	char wndtitle[128];

	for(int i=0;i<xywh.w*xywh.h;i++)
		sp[i].Alloc(e-s+1);

	// �e�t���[������
	for(int n=s;n<=e;n++){

// �����Œ��f����

		// �\���t���[�����X�V
		fp->exfunc->set_frame(editp,n);
		wsprintf(wndtitle,"���S��͒�...  [%d/%d]",n-s+1,e-s+1);
		SetWindowText(GetWindow(fp->hwnd,GW_OWNER),wndtitle);

		// �摜�擾
		PIXEL_YC* pix = fp->exfunc->get_ycp_filtering_cache_ex(fp,editp,n,NULL,NULL);

		// �w�i���ϒl�v�Z
		PIXEL_YC bg;

		if(Cal_BGcolor(bg,pix,xywh,w,fp->track[tTHY])==false)
			// �P��w�i�łȂ��Ƃ��T���v�����Z�b�g���Ȃ�
			continue;
/*
FILE* file;
file=fopen("logoscan.txt","a");
fprintf(file,"%d\n",n+1);
fclose(file);
*/
		// ���S�T���v���Z�b�g
		for(int i=0;i<xywh.h;i++){
			for(int j=0;j<xywh.w;j++){
				PIXEL_YC ptr;
				ptr = pix[(xywh.y+i)*w + xywh.x+j];
				sp[i*xywh.w+j].AddSample(ptr,bg);	// X��:�w�i
			}
		}

	}// �t���[�����������܂�
}

/*--------------------------------------------------------------------
*	�w�i�F�v�Z
*-------------------------------------------------------------------*/
bool Cal_BGcolor(PIXEL_YC& r,PIXEL_YC* pix,XYWH& xywh,int w,int thy)
{
	short* y;	// �w�i�F�z��
	short* cb;
	short* cr;
	int i,n;

	n = 0;

	// (��+����+2)*2
	y  = new short[(xywh.w+xywh.h+2)*2];
	cb = new short[(xywh.w+xywh.h+2)*2];
	cr = new short[(xywh.w+xywh.h+2)*2];

	pix += xywh.x-1 + (xywh.y-1)*w;	// X-1,Y-1�Ɉړ�

	// �����i��j���v
	for(i=0;i<=xywh.w+1;i++){
		y[n]  = pix->y;
		cb[n] = pix->cb;
		cr[n] = pix->cr;
		n++;
		pix++;
	}
	pix += w - i;	// ���̍s��
	// �c��
	for(i=2;i<=xywh.h+1;i++){
		// ����
		y[n]  = pix->y;
		cb[n] = pix->cb;
		cr[n] = pix->cr;
		n++;
		// �E��
		y[n]  = pix[xywh.w+1].y;
		cb[n] = pix[xywh.w+1].cb;
		cr[n] = pix[xywh.w+1].cr;
		n++;

		pix += w;	// ���̍s��
	}
	// �����i���j���v
	for(i=0;i<=xywh.w+1;i++){
		y[n]  = pix->y;
		cb[n] = pix->cb;
		cr[n] = pix->cr;
		n++;
		pix++;
	}

	// �\�[�g���Đ^����ւ�𕽋�
	r.y  = med_average(y,n);
	r.cb = med_average(cb,n);
	r.cr = med_average(cr,n);

	bool ret = true;	// �ԋp�l

	// �ŏ��ƍő傪臒l�ȏ㗣��Ă���ꍇ�A�P��F�łȂ��Ɣ��f
	if((abs(y[0] - y[n-1])>thy*8) ||
	   (abs(cb[0]-cb[n-1])>thy*8) ||
	   (abs(cr[0]-cr[n-1])>thy*8))
			ret = false;

	delete[] y;
	delete[] cb;
	delete[] cr;

	return ret;
}

/*--------------------------------------------------------------------
*	���S�f�[�^���쐬
*-------------------------------------------------------------------*/
void CreateLogoData(FILTER* fp,ScanPixel*& sp)
{
	// ���S�w�b�_�쐬�i���̈ȊO�j
	LOGO_HEADER lgh;
	ZeroMemory(&lgh,sizeof(LOGO_HEADER));
	lgh.x = fp->track[tLOGOX];
	lgh.y = fp->track[tLOGOY];
	lgh.w = fp->track[tLOGOW];
	lgh.h = fp->track[tLOGOH];

	// ���S�f�[�^�̈�m��
	logodata = (void*) new char[LOGO_DATASIZE(&lgh)];
	if(logodata==NULL) throw "�������m�ۂł��܂���ł���";
	*((LOGO_HEADER*)logodata) = lgh;	// �w�b�_�R�s�[

	LOGO_PIXEL* lgp;
	lgp = (LOGO_PIXEL*) ((LOGO_HEADER*)logodata+1);

	for(int i=0;i<lgh.w*lgh.h;i++)
		sp[i].GetLGP(lgp[i]);
}

/*--------------------------------------------------------------------
*	short�^��r�֐��iqsort�p�j
*-------------------------------------------------------------------*/
int comp_short(const void* x,const void* y)
{
	return (*(const short*)x - *(const short*)y);
}

/*--------------------------------------------------------------------
*	�\�[�g���Đ^����ւ�𕽋�
*-------------------------------------------------------------------*/
static short med_average(short* s,int n)
{
	double t  =0.0;
	int    nn =0;

	// �\�[�g����
	qsort(s,n,sizeof(short),comp_short);

	// �^����ւ�𕽋�
	for(int i=n/3;i<n-(n/3);i++,nn++)
		t += s[i];

	t = t / nn + 0.5;

	return ((short)t);
}

//*/
