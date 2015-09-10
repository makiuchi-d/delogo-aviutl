/*====================================================================
* 	��͌��ʃ_�C�A���O			resultdlg.cpp
* 								�ŏI�X�V�F2003/06/17
*===================================================================*/
#include <windows.h>
#include "..\filter.h"
#include "..\logo.h"
#include "resultdlg.h"
#include "resource.h"
#include "..\send_lgd.h"


#define LGD_FILTER  "���S�f�[�^�t�@�C�� (*.lgd)\0*.lgd\0�S�Ẵt�@�C�� (*.*)\0*.*\0"
#define LGD_DEFAULT "*.lgd"


FILTER* dlgfp;	// FILTER�\����
char    defname[32];	// �f�t�H���g���S��

PIXEL* pix;	// �\���p�r�b�g�}�b�v
BITMAPINFO  bmi;
const PIXEL_YC yc_black = {    0,    0,    0 };	// ��
const PIXEL_YC yc_white = { 4080,    0,    0 };	// ��
const PIXEL_YC yc_red   = { 1220, -688, 2040 };	// ��
const PIXEL_YC yc_green = { 2393,-1351,-1707 };	// ��
const PIXEL_YC yc_blue  = {  467, 2040, -333 };	// ��


UINT WM_SEND_LOGO_DATA;	// ���S�f�[�^���M���b�Z�[�W
FILTER* delogofp;	// ���S�����t�B���^FILTER�\����


extern void* logodata;	// ���S�f�[�^�i��͌��ʁj[filter.cpp]
extern char  filter_name[];	// �t�B���^�� [filter.cpp]

//----------------------------
// 	�v���g�^�C�v
//----------------------------
static void Wm_initdialog(HWND hdlg);
static void DispLogo(HWND hdlg,const PIXEL_YC ycbg);
static void idc_save(HWND hdlg);
static void ExportLogoData(char *fname,void *data,HWND hdlg);
static void SendLogoData(HWND hdlg);

/*====================================================================
* 	OptDlgProc()		�R�[���o�b�N�v���V�[�W��
*===================================================================*/
BOOL CALLBACK ResultDlgProc(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg){
		case WM_INITDIALOG:
			Wm_initdialog(hdlg);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDCANCEL:
				case IDC_CLOSE:	// ����{�^��
					if(pix) VirtualFree(pix,0,MEM_RELEASE);
					EndDialog(hdlg,LOWORD(wParam));
					break;

				case IDC_SAVE:	// �ۑ��{�^��
					idc_save(hdlg);
					break;

				case IDC_SEND:	// ���M�{�^��
					SendLogoData(hdlg);
					break;

				//--------------------------------�w�i�F�ݒ�
				case IDC_BLACK:
					DispLogo(hdlg,yc_black);
					return TRUE;

				case IDC_WHITE:
					DispLogo(hdlg,yc_white);
					return TRUE;

				case IDC_RED:
					DispLogo(hdlg,yc_red);
					return TRUE;

				case IDC_GREEN:
					DispLogo(hdlg,yc_green);
					return TRUE;

				case IDC_BLUE:
					DispLogo(hdlg,yc_blue);
					return TRUE;
			}
			break;
	}

	return FALSE;
}


/*--------------------------------------------------------------------
* 	Wm_initdialog()		������
*-------------------------------------------------------------------*/
static void Wm_initdialog(HWND hdlg)
{
	int n;

	// �ő啶�����Z�b�g
	SendDlgItemMessage(hdlg,IDC_EDIT,EM_LIMITTEXT,LOGO_MAX_NAME-2,0);
	// �f�t�H���g���S���Z�b�g
	SetDlgItemText(hdlg,IDC_EDIT,defname);

	pix = NULL;

	// ���S�����t�B���^��T��
	delogofp = NULL;
	for(n=0;(delogofp=(FILTER*)dlgfp->exfunc->get_filterp(n))!=NULL;n++){
		if(lstrcmp(delogofp->name,LOGO_FILTER_NAME)==0){	// ���O�Ŕ���
			// ���S�����t�B���^����������
			WM_SEND_LOGO_DATA = RegisterWindowMessage(wm_send_logo_data);
			return;
		}
	}

	// �݂���Ȃ�������
	delogofp = NULL;
	EnableWindow(GetDlgItem(hdlg,IDC_SEND),FALSE);	// ���M�֎~
}

/*--------------------------------------------------------------------
* 	DispLogo()	���S��\��
*-------------------------------------------------------------------*/
static void DispLogo(HWND hdlg,const PIXEL_YC ycbg)
{
	LOGO_HEADER *lgh;
	LOGO_PIXEL  *lgp;
	PIXEL_YC    yc;
	int   i,j,t;
	HDC   hdc;
	HWND  panel;
	RECT  rec;
	double magnify;	// �\���{��

	lgh = (LOGO_HEADER*)logodata;

	// BITMAPINFO�ݒ�
	ZeroMemory(&bmi,sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = lgh->w + (4-lgh->w%4);	// �S�̔{��
	bmi.bmiHeader.biHeight      = lgh->h;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	// �������Ċm��
	pix = (PIXEL*)VirtualAlloc(pix,bmi.bmiHeader.biWidth*bmi.bmiHeader.biHeight*sizeof(PIXEL)
																	,MEM_COMMIT,PAGE_READWRITE);
	if(pix==NULL){	// �������m�ێ��s
		MessageBox(hdlg,"���������m�ۂł��܂���ł���\nDispLogo()",filter_name,MB_OK|MB_ICONERROR);
		return;	// �������Ȃ�
	}

	(void *)lgp = lgh + 1;

	// RGB�f�[�^�쐬
	for(i=0;i<lgh->h;i++){
		for(j=0;j<lgh->w;j++){
			// �P�x
			yc.y = ((long)ycbg.y*((long)LOGO_MAX_DP-lgp->dp_y) + (long)lgp->y*lgp->dp_y) / LOGO_MAX_DP;
			// �F��(��)
			yc.cb = ((long)ycbg.cb*((long)LOGO_MAX_DP-lgp->dp_cb) + (long)lgp->cb*lgp->dp_cb) / LOGO_MAX_DP;
			// �F��(��)
			yc.cr = ((long)ycbg.cr*((long)LOGO_MAX_DP-lgp->dp_cr) + (long)lgp->cr*lgp->dp_cb) / LOGO_MAX_DP;

			// YCbCr -> RGB
			dlgfp->exfunc->yc2rgb(&pix[bmi.bmiHeader.biWidth*(lgh->h-1-i)+j],&yc,1);

			lgp++;
		}
	}

	// �E�B���h�E�n���h���擾
	panel = GetDlgItem(hdlg,IDC_PANEL);

	// rect�ݒ�
	GetClientRect(panel,&rec);
	rec.left  = 2;
	rec.top   = 8;
	rec.right  -= 3;
	rec.bottom -= 3;

	// �\���摜�̔{���E�ʒu
	if(rec.right-rec.left >= lgh->w*2){	// �������܂鎞
		if(rec.bottom-rec.top >= lgh->h*2)	// ���������܂鎞
			magnify = 2;
		else	// �����̂ݎ��܂�Ȃ�
			magnify = ((double)rec.bottom-rec.top)/lgh->h;
	}
	else {
		if(rec.bottom-rec.top >= lgh->h*2)	// ���̂ݎ��܂�Ȃ�
			magnify = ((double)rec.right-rec.left)/lgh->w;
		else{ // �������������܂�Ȃ�
			magnify = ((double)rec.bottom-rec.top)/lgh->h;// �����Ōv�Z
			magnify = (magnify>((double)rec.right-rec.left)/lgh->w) ? // �{������������
								((double)rec.right-rec.left)/lgh->w : magnify;
		}
	}

	i = (rec.right-rec.left - lgh->w*magnify)/2 + rec.left;	// �����ɕ\������悤��
	j = (rec.bottom-rec.top - lgh->h*magnify)/2 + rec.top;	// left,top���v�Z

	// �f�o�C�X�R���e�L�X�g�擾
	hdc = GetDC(panel);

	SetStretchBltMode(hdc,COLORONCOLOR);
	// 2�{�Ɋg��\��
	StretchDIBits(hdc,i,j,lgh->w*magnify,lgh->h*magnify,
					0,0,lgh->w,lgh->h,pix,&bmi,DIB_RGB_COLORS,SRCCOPY);

	ReleaseDC(panel,hdc);
}

/*--------------------------------------------------------------------
* 	SaveLogoData
*-------------------------------------------------------------------*/
static void idc_save(HWND hdlg)
{
	char filename[MAX_PATH];

	ZeroMemory(defname,32);
	GetDlgItemText(hdlg,IDC_EDIT,defname,LOGO_MAX_NAME);
	if(lstrlen(defname)==0){
		MessageBox(hdlg,"���S������͂��Ă�������",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	// ���S���ݒ�
	defname[LOGO_MAX_NAME-1] = '\0';	// �I�[
	lstrcpy((char*)logodata,defname);

	// �ۑ��t�@�C�����擾
	wsprintf(filename,"%s.lgd",defname);
	if(!dlgfp->exfunc->dlg_get_save_name(filename,LGD_FILTER,filename))
		return;

	ExportLogoData(filename,logodata,hdlg);
}

/*--------------------------------------------------------------------
* 	ExportLogoData()	���S�f�[�^�������o��
*-------------------------------------------------------------------*/
static void ExportLogoData(char *fname,void *data,HWND hdlg)
{
	HANDLE hFile;
	DWORD  dw;
	DWORD  size;
	char   c =1;
	int    s =0;

	// �t�@�C�����J��
	hFile = CreateFile(fname,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==NULL){
		MessageBox(hdlg,"�t�@�C���̃I�[�v���Ɏ��s���܂���",filter_name,MB_OK|MB_ICONERROR);
	}
	SetFilePointer(hFile,0, 0, FILE_BEGIN);	// �擪��

	// �w�b�_��������
	dw = 0;
	WriteFile(hFile,LOGO_FILE_HEADER,31,&dw,NULL);
	if(dw!=31){	// �������ݎ��s
		MessageBox(hdlg,"���S�f�[�^�ۑ��Ɏ��s���܂���(1)",filter_name,MB_OK|MB_ICONERROR);
		s=1;
	}
	else{	// ����
		// �f�[�^���������݁i�K���P�j
		dw = 0;
		WriteFile(hFile,&c,1,&dw,NULL);
		if(dw!=1){	// �������ݎ��s
			MessageBox(hdlg,"���S�f�[�^�ۑ��Ɏ��s���܂���(2)",filter_name,MB_OK|MB_ICONERROR);
			s=1;
		}
		else{	// ����
			// �f�[�^��������
			size = LOGO_DATASIZE(data);	// �f�[�^�T�C�Y�擾
			dw = 0;
			WriteFile(hFile,data,size,&dw,NULL);
			if(dw!=size){
				MessageBox(hdlg,"���S�f�[�^�ۑ��Ɏ��s���܂���(3)",filter_name,MB_OK|MB_ICONERROR);
				s=1;
			}
		}
	}

	CloseHandle(hFile);

	if(s)	// �G���[���������Ƃ�
		DeleteFile(fname);	// �t�@�C���폜
}

/*--------------------------------------------------------------------
* 	SendLogoData()	���S�f�[�^�𑗐M����
*-------------------------------------------------------------------*/
static void SendLogoData(HWND hdlg)
{
	if(!delogofp) return;	// ���S�����t�B���^������
	if(!logodata) return;	// ���S�f�[�^������

	// ���S���ݒ�
	ZeroMemory(defname,32);
	GetDlgItemText(hdlg,IDC_EDIT,defname,LOGO_MAX_NAME);
	if(lstrlen(defname)==0){
		MessageBox(hdlg,"���S������͂��Ă�������",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	defname[LOGO_MAX_NAME-1] = '\0';	// �I�[
	lstrcpy((char*)logodata,defname);

	SendMessage(delogofp->hwnd,WM_SEND_LOGO_DATA,(WPARAM)logodata,0);
}
