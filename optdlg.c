/*====================================================================
* 	�I�v�V�����_�C�A���O			optdlg.c
*===================================================================*/
#include <windows.h>
#include <commctrl.h>
#include "filter.h"
#include "logo.h"
#include "optdlg.h"
#include "resource.h"
#include "editdlg.h"


#define LGD_FILTER  "���S�f�[�^�t�@�C�� (*.lgd)\0*.lgd\0"\
                    "�S�Ẵt�@�C�� (*.*)\0*.*\0"
#define LGD_DEFAULT "*.lgd"


//----------------------------
//	�֐��v���g�^�C�v
//----------------------------
static void Wm_initdialog(HWND hdlg);
static BOOL on_IDOK(HWND,WPARAM);
static BOOL on_IDCANCEL(HWND,WPARAM);
static void on_IDC_ADD(HWND hdlg);
static void on_IDC_DEL(HWND hdlg);
static void on_IDC_EXPORT(HWND hdlg);
static void on_IDC_UP(HWND hdlg);
static void on_IDC_DOWN(HWND hdlg);
static void on_IDC_EDIT(HWND hdlg);

static void AddItem(HWND hdlg,void *data);
void InsertItem(HWND hdlg,int n,void *data);
void DeleteItem(HWND list,int num);
static int  ReadLogoData(char *fname,HWND hdlg);
static void ExportLogoData(char *,void *,HWND);
static void CopyLBtoCB(HWND list,HWND combo);
static void CopyCBtoLB(HWND combo,HWND list);
static void DispLogo(HWND hdlg);
static void set_bgyc(HWND hdlg);
static void RGBtoYCbCr(PIXEL_YC *ycp,const PIXEL *rgb);


//----------------------------
//	�O���[�o���ϐ�
//----------------------------
FILTER *optfp;
HWND    hcb_logo;	// �R���{�{�b�N�X�̃n���h��
HWND    hoptdlg;
PIXEL  *pix;		// �\���p�s�N�Z��
PIXEL_YC bgyc; // �w�i�F

const PIXEL_YC yc_black = {    0,    0,    0 };	// ��
//const PIXEL_YC yc_white = { 4080,    0,    0 };	// ��
//const PIXEL_YC yc_red   = { 1220, -688, 2040 };	// ��
//const PIXEL_YC yc_green = { 2393,-1351,-1707 };	// ��
//const PIXEL_YC yc_blue  = {  467, 2040, -333 };	// ��

void **add_list;
void **del_list;
int  add_num,del_num;
int  add_buf,del_buf;

extern char filter_name[];



/*====================================================================
* 	OptDlgProc()		�R�[���o�b�N�v���V�[�W��
*===================================================================*/
BOOL CALLBACK OptDlgProc(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg){
		case WM_INITDIALOG:
			Wm_initdialog(hdlg);
			break;

		case WM_PAINT:
			DispLogo(hdlg);	// �\��
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam)){
				//------------------------------------------�{�^������
				case IDOK:
					return on_IDOK(hdlg,wParam);

				case IDCANCEL:
					return on_IDCANCEL(hdlg,wParam);

				case IDC_ADD:	// �ǉ�
					on_IDC_ADD(hdlg);
					break;

				case IDC_DEL:	// �폜
					on_IDC_DEL(hdlg);
					break;

				case IDC_EXPORT:	// �����o��
					on_IDC_EXPORT(hdlg);
					break;

				case IDC_EDIT:		// �ҏW
					on_IDC_EDIT(hdlg);
					break;

				case IDC_UP:	// ��
					on_IDC_UP(hdlg);
					break;

				case IDC_DOWN:	// ��
					on_IDC_DOWN(hdlg);
					break;

				//------------------------------------------�w�i�F�ύX


				case IDC_RED:
				case IDC_GREEN:
				case IDC_BLUE:
					//bgyc = yc_red;
					set_bgyc(hdlg);
					DispLogo(hdlg);
					return TRUE;

				//-------------------------------------- ���X�g�{�b�N�X
				case IDC_LIST:
					switch(HIWORD(wParam)){
						case LBN_SELCHANGE:
							DispLogo(hdlg);
							break;

						case LBN_DBLCLK:	// �_�u���N���b�N
							on_IDC_EDIT(hdlg);	// �ҏW
					}
					break;
			}
	}

	return FALSE;
}

/*--------------------------------------------------------------------
* 	Wm_initdialog()	������
*-------------------------------------------------------------------*/
static void Wm_initdialog(HWND hdlg)
{
	hoptdlg = hdlg;
	add_list = malloc(4*sizeof(void *));
	del_list = malloc(4*sizeof(void *));
	add_num = del_num = 0;
	add_buf = del_buf = 4;
	pix = NULL;

	// �R���{�{�b�N�X����A�C�e�����R�s�[
	CopyCBtoLB(hdlg,hcb_logo);
	// RGB�G�f�B�b�g�E�X�s���̃����W�ݒ�
	SendDlgItemMessage(hdlg,IDC_RED,   EM_SETLIMITTEXT, 3,0);
	SendDlgItemMessage(hdlg,IDC_GREEN, EM_SETLIMITTEXT, 3,0);
	SendDlgItemMessage(hdlg,IDC_BLUE,  EM_SETLIMITTEXT, 3,0);
	SendDlgItemMessage(hdlg,IDC_SPINR, UDM_SETRANGE, 0, 255);
	SendDlgItemMessage(hdlg,IDC_SPING, UDM_SETRANGE, 0, 255);
	SendDlgItemMessage(hdlg,IDC_SPINB, UDM_SETRANGE, 0, 255);

	// �w�i�F�ɍ���I��
//	SendDlgItemMessage(hdlg,IDC_BLACK,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
	SetDlgItemInt(hdlg,IDC_RED,  0,FALSE);
	SetDlgItemInt(hdlg,IDC_GREEN,0,FALSE);
	SetDlgItemInt(hdlg,IDC_BLUE, 0,FALSE);
	bgyc = yc_black;

	// ��ԏ�̃��X�g�A�C�e����I��
	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,0,0);

}


/*--------------------------------------------------------------------
* 	on_IDOK()	OK�{�^������
*-------------------------------------------------------------------*/
static BOOL on_IDOK(HWND hdlg,WPARAM wParam)
{
	int i;

	// ���X�g�{�b�N�X����R���{�{�b�N�X�փR�s�[
	CopyLBtoCB(hdlg,hcb_logo);

	// del_list�̃A�C�e���J��
	for(i=0;i<del_num;i++)
		if(del_list[i]) free(del_list[i]);

	if(add_list) free(add_list);
	if(del_list) free(del_list);

	EndDialog(hdlg,LOWORD(wParam));
	hoptdlg = NULL;

	if(pix) VirtualFree(pix,0,MEM_RELEASE);

	return TRUE;
}

/*--------------------------------------------------------------------
* 	on_IDCANCEL()	�L�����Z���{�^������
*-------------------------------------------------------------------*/
static BOOL on_IDCANCEL(HWND hdlg,WPARAM wParam)
{
	int i;

	// add_list�̃A�C�e���J��
	for(i=0;i<add_num;i++)
		if(add_list[i]) free(add_list[i]);

	if(add_list) free(add_list);
	if(del_list) free(del_list);

	EndDialog(hdlg,LOWORD(wParam));
	hoptdlg = NULL;

	if(pix) VirtualFree(pix,0,MEM_RELEASE);

	return TRUE;
}
/*--------------------------------------------------------------------
* 	on_IDC_ADD()	�ǉ��{�^������
*-------------------------------------------------------------------*/
static void on_IDC_ADD(HWND hdlg)
{
	char filename[MAX_PATH];
	BOOL res;

	// ���[�h�t�@�C�����擾
	res = optfp->exfunc->dlg_get_load_name(filename,LGD_FILTER,LGD_DEFAULT);

	if(res==FALSE)	// �L�����Z��
		return;

	// �ǂݍ���
	ReadLogoData(filename,hdlg);
	DispLogo(hdlg);
}

/*--------------------------------------------------------------------
* 	on_IDC_DEL()	�폜�{�^������
*-------------------------------------------------------------------*/
static void on_IDC_DEL(HWND hdlg)
{
	int n;
	int c;

	// �I���A�C�e���ԍ��擾
	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);

	if(n!=LB_ERR)
		DeleteItem(hdlg,n);	// �폜

	// �A�C�e�����擾
	c = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCOUNT,0,0);

	// �J�����g�Z���Z�b�g
	if(c!=0){
		if(c==n) n--;
		SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,n,0);
		DispLogo(hdlg);
	}
}
/*--------------------------------------------------------------------
* 	on_IDC_EXPORT()	�����o���{�^������
*-------------------------------------------------------------------*/
static void on_IDC_EXPORT(HWND hdlg)
{
	char fname[MAX_PATH];
	void *data;
	int  n;
	BOOL res;

	// �J�����g�Z���擾
	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);

	if(n==LB_ERR){	// �I������Ă��Ȃ�
		MessageBox(hdlg,"���S���I������Ă��܂���",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	data = (void *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,n,0);
	if(data==NULL){
		MessageBox(hdlg,"���S�f�[�^�����Ă��܂�",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	// �Z�[�u�t�@�C�����擾
	// �f�t�H���g�t�@�C�����F���S��.lgd
	wsprintf(fname,"%s.lgd",(char *)data);
	res = optfp->exfunc->dlg_get_save_name(fname,LGD_FILTER,fname);

	if(res==FALSE)	// �L�����Z��
		return;

	ExportLogoData(fname,data,hdlg);
}
/*--------------------------------------------------------------------
* 	on_IDC_EDIT()		�ҏW�{�^������
*-------------------------------------------------------------------*/
static void on_IDC_EDIT(HWND hdlg)
{
	int n;

	// �I��ԍ��擾
	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);

	if(n!= LB_ERR)
		DialogBoxParam(optfp->dll_hinst,"EDIT_DLG",hdlg,EditDlgProc,(LPARAM)n);
	else
		MessageBox(hdlg,"���S���I������Ă��܂���",filter_name,MB_OK|MB_ICONERROR);
}

/*--------------------------------------------------------------------
* 	on_IDC_UP()		���{�^������
*-------------------------------------------------------------------*/
static void on_IDC_UP(HWND hdlg)
{
	int   n;
	void *data;
	char  str[128];

	// �I���ʒu�擾
	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);
	if(n==0 || n==LB_ERR){	// ��ԏ�̂Ƃ�or�I������Ă��Ȃ�
		SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,n,0);
		return;
	}

	// �f�[�^�E������擾
	data = (void *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,n,0);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_GETTEXT,n,(LPARAM)str);

	// �폜
	SendDlgItemMessage(hdlg,IDC_LIST,LB_DELETESTRING,n,0);

	// �}��
	n--;	// ���
	SendDlgItemMessage(hdlg,IDC_LIST,LB_INSERTSTRING,n,(LPARAM)str);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETITEMDATA,n,(LPARAM)data);

	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,n,0);
}

/*--------------------------------------------------------------------
* 	on_IDC_DOWN()		���{�^������
*-------------------------------------------------------------------*/
static void on_IDC_DOWN(HWND hdlg)
{
	int   n;
	int   count;
	void *data;
	char  str[128];

	// �I���ʒu�擾
	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);
	count = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCOUNT,0,0);
	if(n==count-1 || n==LB_ERR){	// ��ԉ�or�I������Ă��Ȃ�
		SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,n,0);
		return;
	}

	// �f�[�^�E������擾
	data = (void *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,n,0);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_GETTEXT,n,(LPARAM)str);

	// �폜
	SendDlgItemMessage(hdlg,IDC_LIST,LB_DELETESTRING,n,0);

	// �}��
	n++;	// ���
	SendDlgItemMessage(hdlg,IDC_LIST,LB_INSERTSTRING,n,(LPARAM)str);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETITEMDATA,n,(LPARAM)data);

	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,n,0);
}

/*--------------------------------------------------------------------
* 	ReadLogoData()	���S�f�[�^��ǂݍ���
*-------------------------------------------------------------------*/
static int ReadLogoData(char *fname,HWND hdlg)
{
	HANDLE hFile;
	LOGO_HEADER lgh;
	DWORD readed = 0;
	ULONG ptr;
	void* data;
	unsigned char num;	// �t�@�C���Ɋ܂܂��f�[�^�̐�
	int n;
	int i;
	int same;
	char message[255];

	// �t�@�C���I�[�v��
	hFile = CreateFile(fname,GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile==INVALID_HANDLE_VALUE){
		MessageBox(hdlg,"���S�f�[�^�t�@�C����������܂���",filter_name,MB_OK|MB_ICONERROR);
		return 0;
	}
	if(GetFileSize(hFile, NULL)<=sizeof(LOGO_HEADER)){	// �T�C�Y�m�F
		CloseHandle(hFile);
		MessageBox(hdlg,"���S�f�[�^�t�@�C�����s���ł�",filter_name,MB_OK|MB_ICONERROR);
		return 0;
	}

	SetFilePointer(hFile,31, 0, FILE_BEGIN);	// �擪����31byte��
	ReadFile(hFile,&num,1,&readed,NULL);	// �f�[�^���擾

	n = 0;	// �ǂݍ��݃f�[�^�J�E���^

	for(i=0;i<num;i++){

		// LOGO_HEADER �ǂݍ���
		readed = 0;
		ReadFile(hFile,&lgh,sizeof(LOGO_HEADER),&readed, NULL);
		if(readed!=sizeof(LOGO_HEADER)){
			MessageBox(hdlg,"���S�f�[�^�̓ǂݍ��݂Ɏ��s���܂���",filter_name,MB_OK|MB_ICONERROR);
			break;
		}

		// �������S�����邩
		same = SendDlgItemMessage(hdlg,IDC_LIST,LB_FINDSTRING,-1,(WPARAM)lgh.name);
		if(same!=CB_ERR){
			wsprintf(message,"�����̃��S������܂�\n�u�������܂����H\n\n%s",lgh.name);
			if(MessageBox(hdlg,message,filter_name,MB_YESNO|MB_ICONQUESTION)==IDYES){
				// �폜
				DeleteItem(hdlg,same);
			}
			else{	// �㏑�����Ȃ�
				// �t�@�C���|�C���^��i�߂�
				SetFilePointer(hFile,LOGO_PIXELSIZE(&lgh), 0, FILE_CURRENT);
				continue;
			}
		}

		// �������m��
		data = malloc(LOGO_DATASIZE(&lgh));
		if(data==NULL){
			MessageBox(hdlg,"������������܂���",filter_name,MB_OK|MB_ICONERROR);
			break;
		}

		// LOGO_HEADER�R�s�[
		*((LOGO_HEADER *)data) = lgh;

		ptr = (unsigned long)data + sizeof(LOGO_HEADER);

		// LOGO_PIXEL�ǂݍ���
		readed = 0;
		ReadFile(hFile,(void *)ptr,LOGO_PIXELSIZE(&lgh),&readed,NULL);

		if(LOGO_PIXELSIZE(&lgh)>readed){	// �K�؂�΍�
			readed -= readed % 2;
			ptr    += readed;
			memset((void *)ptr,0,LOGO_PIXELSIZE(&lgh)-readed);
		}

		// ���X�g�{�b�N�X�ɒǉ�
		AddItem(hdlg,data);

		n++;
	}

	CloseHandle(hFile);

	if(n){
		// ���X�g�{�b�N�X�̃J�[�\���ݒ�
		// �ǂݍ��񂾂��̂ň�ԏ��
		i = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCOUNT,0,0) - n;
		SendDlgItemMessage(hdlg,IDC_LIST,LB_SETCURSEL,i,0);
	}

	return n;
}

/*--------------------------------------------------------------------
* 	ExportLogoData()	���S�f�[�^�������o��
*-------------------------------------------------------------------*/
static void ExportLogoData(char *fname,void *data,HWND hdlg)
{
	HANDLE hFile;
	DWORD  dw;
	DWORD  size;
	int    s =0;
	LOGO_FILE_HEADER lfh;

	// �t�@�C�����J��
	hFile = CreateFile(fname,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==NULL){
		MessageBox(hdlg,"�t�@�C���̃I�[�v���Ɏ��s���܂���",filter_name,MB_OK|MB_ICONERROR);
	}
	SetFilePointer(hFile,0, 0, FILE_BEGIN);	// �擪��

	// �w�b�_��������
	ZeroMemory(&lfh,sizeof(lfh));
	strcpy(lfh.str,LOGO_FILE_HEADER_STR);
	lfh.logonum = 1;	// �f�[�^���͕K���P

	dw = 0;
	WriteFile(hFile,&lfh,sizeof(lfh),&dw,NULL);
	if(dw!=sizeof(lfh)){	// �������ݎ��s
		MessageBox(hdlg,"���S�f�[�^�ۑ��Ɏ��s���܂���(1)",filter_name,MB_OK|MB_ICONERROR);
		s=1;
	}
	else{	// ����
		// �f�[�^��������
		size = LOGO_DATASIZE(data);	// �f�[�^�T�C�Y�擾
		dw = 0;
		WriteFile(hFile,data,size,&dw,NULL);
		if(dw!=size){
			MessageBox(hdlg,"���S�f�[�^�ۑ��Ɏ��s���܂���(2)",filter_name,MB_OK|MB_ICONERROR);
			s=1;
		}
	}

	CloseHandle(hFile);

	if(s)	// �G���[���������Ƃ�
		DeleteFile(fname);	// �t�@�C���폜
}


/*--------------------------------------------------------------------
* 	AddItem()	���X�g�A�C�e����ǉ�
*-------------------------------------------------------------------*/
static void AddItem(HWND hdlg,void *data)
{
	int n;

	n = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCOUNT,0,0);

	SendDlgItemMessage(hdlg,IDC_LIST,LB_INSERTSTRING,n,(LPARAM)(char *)data);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETITEMDATA,n,(LPARAM)data);

	if(add_buf==add_num){	// �o�b�t�@�������ς��̂Ƃ�
		add_buf += 4;
		add_list = realloc(add_list,add_buf*sizeof(void*));
	}

	add_list[add_num] = data;
	add_num++;
}

/*--------------------------------------------------------------------
* 	InsertItem()	���X�g�A�C�e����}��
*-------------------------------------------------------------------*/
void InsertItem(HWND hdlg,int n,void *data)
{
	SendDlgItemMessage(hdlg,IDC_LIST,LB_INSERTSTRING,n,(LPARAM)(char *)data);
	SendDlgItemMessage(hdlg,IDC_LIST,LB_SETITEMDATA,n,(LPARAM)data);

	if(add_buf==add_num){	// �o�b�t�@�������ς��̂Ƃ�
		add_buf += 4;
		add_list = realloc(add_list,add_buf*sizeof(void*));
	}

	add_list[add_num] = data;
	add_num++;
}


/*--------------------------------------------------------------------
* 	DeleteItem()	���X�g�A�C�e�����폜����
*-------------------------------------------------------------------*/
void DeleteItem(HWND hdlg,int num)
{
	if(del_buf==del_num){	// �o�b�t�@�������ς��̂Ƃ�
		del_buf += 4;
		del_list = realloc(del_list,del_buf*sizeof(void *));
	}

	del_list[del_num] = (void *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,num,0);
	del_num++;

	SendDlgItemMessage(hdlg,IDC_LIST,LB_DELETESTRING,num,0);
}

/*--------------------------------------------------------------------
* 	CopyLBtoCB()	���X�g�{�b�N�X����R���{�{�b�N�X�փR�s�[
*-------------------------------------------------------------------*/
static void CopyLBtoCB(HWND hdlg,HWND combo)
{
	int   num;
	int   i;
	void *data;
	char  str[128];

	// �R���{�{�b�N�X�N���A
	num = SendMessage(combo,CB_GETCOUNT,0,0);
	for(i=0;i<num;i++)
		SendMessage(combo,CB_DELETESTRING,0,0);

	// �R�s�[
	num = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCOUNT,0,0);
	for(i=0;i<num;i++){
		// �A�C�e���擾
		data = (void *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,i,0);
		SendDlgItemMessage(hdlg,IDC_LIST,LB_GETTEXT,i,(LPARAM)str);

		// �R���{�{�b�N�X�ɃZ�b�g
		SendMessage(combo,CB_INSERTSTRING,i,(LPARAM)str);
		SendMessage(combo,CB_SETITEMDATA,i,(LPARAM)data);
	}
}

/*--------------------------------------------------------------------
* 	CopyCBtoLB()	�R���{�{�b�N�X���烊�X�g�{�b�N�X�փR�s�[
*-------------------------------------------------------------------*/
static void CopyCBtoLB(HWND hdlg,HWND combo)
{
	int   num;
	int   i;
	void *data;
	char  str[128];

	// �R�s�[
	num = SendMessage(combo,CB_GETCOUNT,0,0);
	for(i=0;i<num;i++){
		// �A�C�e���擾
		data = (void *)SendMessage(combo,CB_GETITEMDATA,i,0);
		SendMessage(combo,CB_GETLBTEXT,i,(LPARAM)str);

		// ���X�g�{�b�N�X�ɃZ�b�g
		SendDlgItemMessage(hdlg,IDC_LIST,LB_INSERTSTRING,i,(LPARAM)str);
		SendDlgItemMessage(hdlg,IDC_LIST,LB_SETITEMDATA,i,(LPARAM)data);
	}
}

/*--------------------------------------------------------------------
* 	DispLogo()	���S��\��
*-------------------------------------------------------------------*/
static void DispLogo(HWND hdlg)
{
	BITMAPINFO  bmi;
	LOGO_HEADER *lgh;
	LOGO_PIXEL  *lgp;
	PIXEL_YC    yc;
	PIXEL       rgb;
	int   i,j;
	HDC   hdc;
	HWND  panel;
	RECT  rec;
	int   temp;
	double magnify;	// �\���{��

	// �I�����S�f�[�^�擾
	i = SendDlgItemMessage(hdlg,IDC_LIST,LB_GETCURSEL,0,0);
	if(i==LB_ERR)		return;	// �I������Ă��Ȃ����������Ȃ�

	lgh = (LOGO_HEADER *)SendDlgItemMessage(hdlg,IDC_LIST,LB_GETITEMDATA,i,0);

	// BITMAPINFO�ݒ�
	ZeroMemory(&bmi,sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = lgh->w + (4-lgh->w%4);	// �S�̔{��
	bmi.bmiHeader.biHeight      = lgh->h;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	// �������m��
	pix = VirtualAlloc(pix,bmi.bmiHeader.biWidth*bmi.bmiHeader.biHeight*sizeof(PIXEL),MEM_COMMIT,PAGE_READWRITE);
	if(pix==NULL){	// �������m�ێ��s
		return;	// �������Ȃ�
	}

	(void *)lgp = lgh + 1;

	// RGB�f�[�^�쐬
	for(i=0;i<lgh->h;i++){
		for(j=0;j<lgh->w;j++){
			// �P�x
			yc.y  = (bgyc.y*(LOGO_MAX_DP-lgp->dp_y) + lgp->y*lgp->dp_y) / LOGO_MAX_DP;
			// �F��(��)
			yc.cb = (bgyc.cb*(LOGO_MAX_DP-lgp->dp_cb) + lgp->cb*lgp->dp_cb) / LOGO_MAX_DP;
			// �F��(��)
			yc.cr = (bgyc.cr*(LOGO_MAX_DP-lgp->dp_cr) + lgp->cr*lgp->dp_cr) / LOGO_MAX_DP;

			// YCbCr -> RGB
			optfp->exfunc->yc2rgb(&pix[(bmi.bmiHeader.biWidth)*(lgh->h-1-i)+j],&yc,1);

			lgp++;
		}
	}

	// rect�ݒ�
	panel = GetDlgItem(hdlg,IDC_PANEL);
	GetClientRect(panel,&rec);
	rec.left  = 2;
	rec.top   = 8;	// FONT-1
	rec.right  -= 3;
	rec.bottom -= 3;

	// �f�o�C�X�R���e�L�X�g�擾
	hdc = GetDC(panel);

	// �h��Ԃ�
	FillRect(hdc,&rec,(HBRUSH)(COLOR_ACTIVEBORDER+1));


	// �\���摜�̔{��
	if(rec.right-rec.left >= lgh->w*1.5){	// �������܂鎞
		if(rec.bottom-rec.top >= lgh->h*1.5)	// ���������܂鎞
			magnify = 1.5;
		else	// �����̂ݎ��܂�Ȃ�
			magnify = ((double)rec.bottom-rec.top)/lgh->h;
	}
	else {
		if(rec.bottom-rec.top >= lgh->h*1.5)	// ���̂ݎ��܂�Ȃ�
			magnify = ((double)rec.right-rec.left)/lgh->w;
		else{ // �������������܂�Ȃ�
			magnify = ((double)rec.bottom-rec.top)/lgh->h;// �����Ōv�Z
			magnify = (magnify>((double)rec.right-rec.left)/lgh->w) ? // �{������������
								((double)rec.right-rec.left)/lgh->w : magnify;
		}
	}

	i = (rec.right-rec.left - lgh->w*magnify +1)/2 + rec.left;	// �����ɕ\������悤��
	j = (rec.bottom-rec.top - lgh->h*magnify +1)/2 + rec.top;	// left,top���v�Z

	SetStretchBltMode(hdc,COLORONCOLOR);
	// �g��\��
	StretchDIBits(hdc,i,j,lgh->w*magnify,lgh->h*magnify,
					0,0,lgh->w,lgh->h,pix,&bmi,DIB_RGB_COLORS,SRCCOPY);

	ReleaseDC(panel,hdc);
}

/*--------------------------------------------------------------------
* 	set_bgyc()	�v���r���[�w�i�F���擾
*-------------------------------------------------------------------*/
static void set_bgyc(HWND hdlg)
{
	BOOL  trans;
	int   t;
	PIXEL p;

	// RGB�l�擾
	t = GetDlgItemInt(hdlg,IDC_BLUE,&trans,FALSE);
	if(trans==FALSE) p.b = 0;
	else if(t > 255) p.b = 255;
	else if(t < 0)   p.b = 0;
	else  p.b = t;
	if(t != p.b)
		SetDlgItemInt(hdlg,IDC_BLUE ,p.b,FALSE);

	t = GetDlgItemInt(hdlg,IDC_GREEN,&trans,FALSE);
	if(trans==FALSE) p.g = 0;
	else if(t > 255) p.g = 255;
	else if(t < 0)   p.g = 0;
	else  p.g = t;
	if(t != p.g)
		SetDlgItemInt(hdlg,IDC_GREEN,p.g,FALSE);

	t = GetDlgItemInt(hdlg,IDC_RED,&trans,FALSE);
	if(trans==FALSE) p.r = 0;
	else if(t > 255) p.r = 255;
	else if(t < 0)   p.r = 0;
	else  p.r = t;
	if(t != p.r)
		SetDlgItemInt(hdlg,IDC_RED  ,p.r,FALSE);

	// RGB -> YCbCr
	RGBtoYCbCr(&bgyc,&p);
}

/*--------------------------------------------------------------------
* 	RGBtoYCbCr()
*-------------------------------------------------------------------*/
static void RGBtoYCbCr(PIXEL_YC *ycp,const PIXEL *rgb)
{
	ycp->y  =  0.2989*4096/256*rgb->r + 0.5866*4096/256*rgb->g + 0.1145*4096/256*rgb->b +0.5;
	ycp->cb = -0.1687*4096/256*rgb->r - 0.3312*4096/256*rgb->g + 0.5000*4096/256*rgb->b +0.5;
	ycp->cr =  0.5000*4096/256*rgb->r - 0.4183*4096/256*rgb->g - 0.0816*4096/256*rgb->b +0.5;
}

//*/
