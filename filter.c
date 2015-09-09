/*********************************************************************
* 	���ߐ����S�iBS�}�[�N�Ƃ��j�����t�B���^
* 								ver. 0
* 
* 2003
* 	02/01:	����J�n
* 	02/02:	�g���f�[�^�̈���g���ƃv���t�@�C���؂�ւ�����AviUtl�������΂�B
* 	      	�Ȃ��H�o�O�H�ǂ����悤�B
* 	      	�Ǝv������A�̈�T�C�Y�ɐ���������̂ˁc SDK�ɂ͈ꌾ�������ĂȂ����ǁB
* 	      	�������|�|!!�i�a�����j������Ɗ����B
* 	      	BS���S���ċP�x�����̕ω��Ȃ̂����BRGB�ł��ƐF�ς��킯���B
* 	02/06:	�v���t�@�C���̓r���؂�ւ��ɉ��Ή��B
* 	02/08:	BS2���S�����B�i�e�X�g�pver.1�j
* 	02/11:	YCbCr��������ł���悤�ɂ����B
* 	02/17:	�����I�ɊO���t�@�C���ǂݍ��݋@�\�𓋍ځB�i�T�܂Łj
* 	03/02:	���S�f�[�^�ۑ��̈�֌W�i�|�C���^���R���{�A�C�e���Ɋ֘A�t���j
* 	03/24:	���S�f�[�^�ǂݍ��݊֌W
* 	03/25:	�t�B���^�{�̂ƃ��S�f�[�^��؂藣�����B
* 	03/26:	���S�f�[�^�̊Ǘ������S�ɃR���{�A�C�e���Ɗ֘A�t��
* 	03/29:	���S�f�[�^���o�̓_�C�A���O�i�I�v�V�����_�C�A���O�j�쐬
* 	03/30:	���S�t�����[�h�ǉ�
* 	04/03:	���S�f�[�^�̍\����ύX�idp��dp_y,dp_cb,dp_cr�ɕ����j
* 			�I�v�V�����_�C�A���O�Ƀv���r���[�@�\��ǉ�
* 	04/06:	���S��̓A���S���Y���ƁA�����p�����t�B���^�̃e�X�g�ł�
* 			�����������ߌv�Z��������ɍœK���B�i�s�v�ȕ�������炵���j
* 	04/07:	���S�v���r���[�̔w�i�F��ύX�ł���悤�ɂ����B
* 	04/09:	��̓v���O�C������̃f�[�^����M�o����悤�ɂ����B
* 			�[�x�����̕��@��ύX(ofset->gain)
* 			�v���r���[�Řg����͂ݏo���Ȃ��悤�ɂ����B
* 	04/20:	�t�B���^���ύX�B���S�t�����[�h���ꎞ�p�~�B
* 	04/28:	1/4�P�ʈʒu���������B���S�t�����[�h�����i����܂�Ӗ��Ȃ����ǁj
* 
*********************************************************************/

/* ToDo:
* 	�EBS1/2���A�����ȃ��S�̎���
* 	�E���S�f�[�^�̍쐬�E�ҏW�@�\
* 
*  MEMO:
* 	�E���S�̊g��k�����[�`���������Ȃ��Ƃ��߂��Ȃ��B
* 		���K�v�Ȃ����B���ʂ͎������Ȃ��B
* 	�E���S�쐬�E�ҏW�͕ʃA�v���ɂ��Ă��܂������c
* 	�E���S�������[�h�ƃ��S�t�����[�h��؂�ւ�����悤�ɂ��悤����
* 		���t���Ă݂�
* 	�E��̓v���O����f�[�^���󂯎��ɂ́c�Ǝ�WndMsg�o�^����SendMessage�ő����Ă��炤
* 		�������Ɠ������B
* 	�E���S�ɂP�s�N�Z�������̃Y��������B1/4�s�N�Z���ł̈ʒu�������K�v�����B
* 		����������
*/
#include <windows.h>
#include <commctrl.h>
#include <math.h>
#include "filter.h"
#include "logo.h"
#include "optdlg.h"
#include "resource.h"
#include "send_lgd.h"


#define ID_BUTTON_OPTION 40001
#define ID_COMBO_LOGO    40002

#define Abs(x) ((x>0)? x:-x)


#define LDP_KEY     "logofile"
#define LDP_DEFAULT "logodata.ldp"
#define LDP_FILTER  "���S�f�[�^�p�b�N(*.ldp)\0*.ldp\0AllFiles(*.*)\0*.*\0"


// �_�C�A���O�A�C�e��
typedef struct {
	HFONT font;
	HWND  cb_logo;
	HWND  bt_opt;
} FILTER_DIALOG;

FILTER_DIALOG dialog;

char  logodata_file[MAX_PATH];	// ���S�f�[�^�t�@�C����(INI�ɕۑ�)
UINT  WM_SEND_LOGO_DATA;

//----------------------------
//	�v���g�^�C�v�錾
//----------------------------
static void on_wm_filter_exit(FILTER* fp);
static void init_dialog(HWND hwnd,HINSTANCE hinst);
static BOOL set_exdata(FILTER *fp,const void *ptr);
static void update_cb_logo(char *name);
static void change_param(FILTER* fp);
static void set_cb_logo(FILTER* fp);
static int  set_combo_item(void* data);
static void del_combo_item(int num);
static void read_logo_pack(char *logodata_file,FILTER *fp);
static void set_sended_data(void* logodata,FILTER* fp);
static void create_adj_exdata(FILTER *fp,const LOGO_HEADER *data);

static BOOL on_option_button(FILTER* fp);

BOOL func_proc_eraze_logo(FILTER *fp,FILTER_PROC_INFO *fpip);
BOOL func_proc_add_logo(FILTER *fp,FILTER_PROC_INFO *fpip);

//----------------------------
//	FILTER_DLL�\����
//----------------------------
char filter_name[] = LOGO_FILTER_NAME;
char filter_info[] = LOGO_FILTER_NAME" ver0 by MakKi";
#define track_N 8
#if track_N
TCHAR *track_name[]   = { "�ʒu X", "�ʒu Y", "Y�[�x","Cb�[�x","Cr�[�x", "Y", "Cb", "Cr" };	// �g���b�N�o�[�̖��O
int   track_default[] = {   0,   0,   0,   0,   0,   0,   0,   0 };	// �g���b�N�o�[�̏����l
int   track_s[]       = { -200, -200, -128, -128, -128, -100, -100, -100 };	// �g���b�N�o�[�̉����l
int   track_e[]       = {  200,  200,  128,  128,  128,  100,  100,  100 };	// �g���b�N�o�[�̏���l
#endif
#define check_N 2
#if check_N
TCHAR *check_name[]   = { "���S�t�����[�h","���S�������[�h" };	// �`�F�b�N�{�b�N�X
int   check_default[] = { 0, 1 };	// �f�t�H���g
#endif


#define LOGO_X      0
#define LOGO_Y      1
#define LOGO_YDP    2
#define LOGO_CBDP   3
#define LOGO_CRDP   4
#define LOGO_PY     5
#define LOGO_CB     6
#define LOGO_CR     7

// �ݒ�E�B���h�E�̍���
#define WND_Y (67+24*track_N+20*check_N)

char ex_data[LOGO_MAXSIZE];	// �g���f�[�^�̈�

FILTER_DLL filter = {
	FILTER_FLAG_WINDOW_SIZE |	//	�t�B���^�̃t���O
	FILTER_FLAG_RADIO_BUTTON |
	FILTER_FLAG_EX_DATA |
	FILTER_FLAG_EX_INFORMATION,
	320,WND_Y,			// �ݒ�E�C���h�E�̃T�C�Y
	filter_name,    	// �t�B���^�̖��O
#if track_N
	track_N,        	// �g���b�N�o�[�̐�
	track_name,     	// �g���b�N�o�[�̖��O�S
	track_default,  	// �g���b�N�o�[�̏����l�S
	track_s,track_e,	// �g���b�N�o�[�̐��l�̉������
#else
	0,NULL,NULL,NULL,NULL,
#endif
#if check_N
	check_N,      	// �`�F�b�N�{�b�N�X�̐�
	check_name,   	// �`�F�b�N�{�b�N�X�̖��O�S
	check_default,	// �`�F�b�N�{�b�N�X�̏����l�S
#else
	0,NULL,NULL,
#endif
	func_proc,   	// �t�B���^�����֐�
	NULL,NULL,   	// �J�n��,�I�����ɌĂ΂��֐�
	NULL,        	// �ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐�
	func_WndProc,	// �ݒ�E�B���h�E�v���V�[�W��
	NULL,NULL,   	// �V�X�e���Ŏg�p
	ex_data,     	// �g���f�[�^�̈�
	0,	// �g���f�[�^�T�C�Y
	filter_info, 	// �t�B���^���
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
	if(fp->check[1])
		return func_proc_eraze_logo(fp,fpip);
	else
		return func_proc_add_logo(fp,fpip);
}

/*--------------------------------------------------------------------
* 	func_proc_eraze_logo()	���S�������[�h
*-------------------------------------------------------------------*/
BOOL func_proc_eraze_logo(FILTER* fp,FILTER_PROC_INFO *fpip)
{
	int   i,j;
	int   y,cb,cr;
	PIXEL_YC    *ptr;
	LOGO_HEADER *lgh;
	LOGO_PIXEL  *lgp;
	double dp,temp;

	if(fp->ex_data_ptr==NULL)	// exdata����̎��������Ȃ�
		return FALSE;

	lgh = fp->ex_data_ptr;

	// LOGO_PIXEL�f�[�^�ւ̃|�C���^
	(void *)lgp = lgh + 1;

	// ����̈ʒu�ֈړ�
	ptr = fpip->ycp_edit;
	ptr += lgh->x + lgh->y * fpip->max_w;

	for(i=0;i < lgh->h;i++){
		for(j=0;j < lgh->w;j++){

			if(ptr >= fpip->ycp_edit &&	// ��ʓ��̎��̂ݏ���
			   ptr < fpip->ycp_edit + fpip->max_w*fpip->h)
			{
				// �P�x
				dp = lgp->dp_y * pow(2,(double)fp->track[LOGO_YDP]/128);	// ����
				y  = lgp->y + fp->track[LOGO_PY]*16;
				if(dp==LOGO_MAX_DP) dp++;	// 0�ł̏��Z���
				temp = ((double)ptr->y*LOGO_MAX_DP - y*dp) / (LOGO_MAX_DP - dp) +0.5;	// �t�Z
				if  (temp>4096) temp = 4096;	// �͈̓`�F�b�N
				else if(temp<0) temp = 0;
				ptr->y = temp;

				// �F��(��)
				dp = lgp->dp_cb * pow(2,(double)fp->track[LOGO_CBDP]/128);
				cb = lgp->cb    + fp->track[LOGO_CB]*16;
				if(dp==LOGO_MAX_DP) dp++;	// 0�ł̏��Z���
				temp = ((double)ptr->cb*LOGO_MAX_DP - cb*dp) / (LOGO_MAX_DP - dp) +0.5;
				if      (temp>2048) temp =  2048;	// �͈̓`�F�b�N
				else if(temp<-2048) temp = -2048;
				ptr->cb = temp;

				// �F��(��)
				dp = lgp->dp_cr * pow(2,(double)fp->track[LOGO_CRDP]/128);
				cr = lgp->cr   + fp->track[LOGO_CR]*16;
				if(dp==LOGO_MAX_DP) dp++;	// 0�ł̏��Z���
				temp = ((double)ptr->cr*LOGO_MAX_DP - cr*dp) / (LOGO_MAX_DP - dp) +0.5;
				if      (temp>2048) temp =  2048;	// �͈̓`�F�b�N
				else if(temp<-2048) temp = -2048;
				ptr->cr = temp;

			}	// if��ʓ�

			ptr++;	// �ׂ��
			lgp++;
		}
		// ���̃��C����
		ptr += fpip->max_w - j;
	}

	return TRUE;
}

/*--------------------------------------------------------------------
* 	func_proc_add_logo()	���S�t�����[�h
*-------------------------------------------------------------------*/
BOOL func_proc_add_logo(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	int   i,j;
	int   y,cb,cr;
	PIXEL_YC    *ptr;
	LOGO_HEADER *lgh;
	LOGO_PIXEL  *lgp;
	double dp,temp;

	if(fp->ex_data_ptr==NULL)	// exdata����̎��������Ȃ�
		return FALSE;

	lgh = fp->ex_data_ptr;

	// LOGO_PIXEL�f�[�^�ւ̃|�C���^
	(void *)lgp = lgh + 1;

	// ����̈ʒu�ֈړ�
	ptr = fpip->ycp_edit;
	ptr += lgh->x + lgh->y * fpip->max_w;

	for(i=0;i < lgh->h;i++){
		for(j=0;j < lgh->w;j++){

			if(ptr >= fpip->ycp_edit &&	// ��ʓ��̎��̂ݏ���
			   ptr < fpip->ycp_edit + fpip->max_w*fpip->h)
			{

				// �P�x
				dp = lgp->dp_y * pow(2,(double)fp->track[LOGO_YDP]/128);	// ����(2��x/128��)
				y  = lgp->y    + fp->track[LOGO_PY]*16;
				temp  = ((double)ptr->y*(LOGO_MAX_DP-dp) + y*dp) / LOGO_MAX_DP +0.5;	// ���S�t��
				if  (temp>4096) temp = 4096;	// �͈̓`�F�b�N
				else if(temp<0) temp = 0;
				ptr->y = temp;


				// �F��(��)
				dp = lgp->dp_cb * pow(2,(double)fp->track[LOGO_CBDP]/128);
				cb = lgp->cb    + fp->track[LOGO_CB]*16;
				temp  = ((double)ptr->cb*(LOGO_MAX_DP-dp) + cb*dp) / LOGO_MAX_DP +0.5;
				if     (temp> 2048) temp =  2048;	// �͈̓`�F�b�N
				else if(temp<-2048) temp = -2048;
				ptr->cb = temp;

				// �F��(��)
				dp = lgp->dp_cr * pow(2,(double)fp->track[LOGO_CRDP]/128);
				cr = lgp->cr   + fp->track[LOGO_CR]*16;
				temp  = ((double)ptr->cr*(LOGO_MAX_DP-dp) + cr*dp) / LOGO_MAX_DP +0.5;
				if     (temp> 2048) temp =  2048;	// �͈̓`�F�b�N
				else if(temp<-2048) temp = -2048;
				ptr->cr = temp;

			}	// if��ʓ�

			ptr++;	// �ׂ��
			lgp++;
		}
		// ���̃��C����
		ptr += fpip->max_w - j;
	}

	return TRUE;
}

/*====================================================================
*	�ݒ�E�B���h�E�v���V�[�W��
*===================================================================*/
BOOL func_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void *editp, FILTER *fp )
{
	static UINT  WM_SEND_LOGO_DATA;

	if(message==WM_SEND_LOGO_DATA){	// ���S�f�[�^��M
		set_sended_data((void *)wParam,fp);
		return TRUE;
	}

	switch(message){
		case WM_FILTER_INIT:	// ������
			init_dialog(hwnd,fp->dll_hinst);
			set_cb_logo(fp);
			// ���S�f�[�^��M���b�Z�[�W�o�^
			WM_SEND_LOGO_DATA = RegisterWindowMessage(wm_send_logo_data);
			return TRUE;

		case WM_FILTER_EXIT:	// �I��
			on_wm_filter_exit(fp);
			if(hoptdlg)	// �I�v�V�����_�C�A���O�I��
				SendMessage(hoptdlg,WM_COMMAND,IDCANCEL,0);
			break;

		case WM_FILTER_CHANGE_ACTIVE:	// �L�������ύX
			return set_exdata(fp,NULL);

		case WM_FILTER_UPDATE:	// �t�B���^�X�V
		case WM_FILTER_SAVE_END:	// �Z�[�u�I��
			// �R���{�{�b�N�X�\���X�V
			update_cb_logo(((LOGO_HEADER *)ex_data)->name);
			break;

		case WM_FILTER_CHANGE_PARAM:	// �p�����[�^�ύX
			change_param(fp);
			return TRUE;

		//---------------------------------------------�{�^������
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case ID_BUTTON_OPTION:	// �I�v�V�����{�^��
					return on_option_button(fp);

				case ID_COMBO_LOGO:	// �R���{�{�b�N�X
					switch(HIWORD(wParam)){
						case CBN_SELCHANGE:	// �I��ύX
							change_param(fp);
							return TRUE;
					}
			}
			break;

		case WM_KEYUP:	// ���C���E�B���h�E�֑���
		case WM_KEYDOWN:
		case WM_MOUSEWHEEL:
			SendMessage(GetWindow(hwnd, GW_OWNER), message, wParam, lParam);
			break;
	}

	return FALSE;
}

/*--------------------------------------------------------------------
* 	on_wm_filter_exit()		�I������
*-------------------------------------------------------------------*/
static void on_wm_filter_exit(FILTER* fp)
{
	int    i,num;
	char   n;
	DWORD  dw;
	HANDLE hFile;
	void*  data;

	if(lstrlen(logodata_file)==0){	// ���S�f�[�^�t�@�C�������Ȃ��Ƃ�
		lstrcpy(logodata_file,LDP_DEFAULT);	// �f�t�H���g�̃��S�f�[�^�t�@�C����
	}

	// �o�^����Ă���A�C�e���̐�
	num = SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0);

	// �t�@�C���I�[�v��
	hFile = CreateFile(logodata_file,GENERIC_WRITE, 0, 0,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile,0, 0, FILE_BEGIN);	// �擪��

	// �w�b�_��������
	dw = 0;
	WriteFile(hFile,LOGO_FILE_HEADER,32,&dw,NULL);
	if(dw!=32){	// �������ݎ��s
		MessageBox(fp->hwnd,"���S�f�[�^�ۑ��Ɏ��s���܂���(1)",filter_name,MB_OK|MB_ICONERROR);
	}
	else{	// ����
		n = 0;
		// �e�f�[�^��������
		for(i=0;i<num;i++){
			dw = 0;
			data = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,i,0);	// �f�[�^�̃|�C���^�擾
			WriteFile(hFile,data,LOGO_DATASIZE(data),&dw,NULL);
			if(dw != LOGO_DATASIZE(data)){
				MessageBox(fp->hwnd,"���S�f�[�^�ۑ��Ɏ��s���܂���(2)",filter_name,MB_OK|MB_ICONERROR);
				break;
			}
			n++;
		}

		SetFilePointer(hFile,31, 0, FILE_BEGIN);	// �擪����31byte��
		dw = 0;
		WriteFile(hFile,&n,1,&dw,NULL);
		if(dw!=1)
			MessageBox(fp->hwnd,"���S�f�[�^�ۑ��Ɏ��s���܂���(3)",filter_name,MB_OK|MB_ICONERROR);
	}

	CloseHandle(hFile);

	// INI�Ƀ��S�f�[�^�t�@�C�����ۑ�
	fp->exfunc->ini_save_str(fp,LDP_KEY,logodata_file);

	for(i=0;i<num;i++){	// logo_data�J��
		data = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,i,0);
		if(data) free(data);
	}

}

/*--------------------------------------------------------------------
*	init_dialog()		�_�C�A���O�A�C�e�������
*		�E�R���{�{�b�N�X
*		�E�I�v�V�����{�^��
*-------------------------------------------------------------------*/
static void init_dialog(HWND hwnd,HINSTANCE hinst)
{
#define ITEM_Y (19+24*track_N+20*check_N)

	// �t�H���g�쐬
	dialog.font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// �R���{�{�b�N�X
	dialog.cb_logo = CreateWindow("COMBOBOX", "", WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL,
									60,ITEM_Y, 175,100, hwnd, (HMENU)ID_COMBO_LOGO, hinst, NULL);
	SendMessage(dialog.cb_logo, WM_SETFONT, (WPARAM)dialog.font, 0);

	// �I�v�V�����{�^��
	dialog.bt_opt = CreateWindow("BUTTON", "�I�v�V����", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_VCENTER,
									240,ITEM_Y, 63,20, hwnd, (HMENU)ID_BUTTON_OPTION, hinst, NULL);
	SendMessage(dialog.bt_opt, WM_SETFONT, (WPARAM)dialog.font, 0);
}

/*--------------------------------------------------------------------
*	set_exdata()		�g���̈�Ƀ��S�f�[�^���R�s�[����
*-------------------------------------------------------------------*/
static BOOL set_exdata(FILTER *fp,const void *ptr)
{
	if(ptr)	// ptr��NULL�łȂ��Ƃ�
		create_adj_exdata(fp,ptr);	// �ʒu�������S�f�[�^�쐬 (ex_data�ɃR�s�[)

	if(fp->exfunc->is_filter_active(fp)){
		// �t�B���^�L��
		fp->ex_data_ptr = ex_data;
		fp->ex_data_size = LOGO_DATASIZE(fp->ex_data_ptr);
		return TRUE;
	}
	else{	// ����
		fp->ex_data_ptr = NULL;
		fp->ex_data_size = 0;
		return FALSE;
	}
}
/*--------------------------------------------------------------------
*	create_adj_exdata()		�ʒu�������S�f�[�^�쐬
*-------------------------------------------------------------------*/
static void create_adj_exdata(FILTER *fp,const LOGO_HEADER *data)
{
	int  i,j;
	int  w,h;
	int  temp;
	int  adjx,adjy;
	LOGO_PIXEL *df;
	LOGO_PIXEL *ex;


	// ���S���R�s�[
	lstrcpy(ex_data,data->name);

	// ������W�X�V
	((LOGO_HEADER *)ex_data)->x = data->x +(int)(fp->track[LOGO_X]+200)/4 -50;
	((LOGO_HEADER *)ex_data)->y = data->y +(int)(fp->track[LOGO_Y]+200)/4 -50;

	((LOGO_HEADER *)ex_data)->w = w = data->w + 1;	// ���A�������P���₷
	((LOGO_HEADER *)ex_data)->h = h = data->h + 1;

	// LOGO_PIXEL�̐擪
	(void *)df  = (void *)(data +1);
	(void *)ex = (LOGO_HEADER *)ex_data +1;

	adjx = (fp->track[LOGO_X]+200) % 4;	// �ʒu�[��
	adjy = (fp->track[LOGO_Y]+200) % 4;

	//----------------------------------------------------- ��ԏ�̗�
	ex[0].dp_y  = df[0].dp_y *(4-adjx)*(4-adjy)/16;	// ���[
	ex[0].dp_cb = df[0].dp_cb*(4-adjx)*(4-adjy)/16;
	ex[0].dp_cr = df[0].dp_cr*(4-adjx)*(4-adjy)/16;
	ex[0].y  = df[0].y;
	ex[0].cb = df[0].cb;
	ex[0].cr = df[0].cr;
	for(i=1;i<w-1;i++){									//��
		// Y
		ex[i].dp_y = (df[i-1].dp_y*adjx*(4-adjy)
							+ df[i].dp_y*(4-adjx)*(4-adjy)) /16;
		if(ex[i].dp_y)
			ex[i].y  = (df[i-1].y*Abs(df[i-1].dp_y)*adjx*(4-adjy)
					+ df[i].y * Abs(df[i].dp_y)*(4-adjx)*(4-adjy))
				/(Abs(df[i-1].dp_y)*adjx*(4-adjy) + Abs(df[i].dp_y)*(4-adjx)*(4-adjy));
		// Cb
		ex[i].dp_cb = (df[i-1].dp_cb*adjx*(4-adjy)
							+ df[i].dp_cb*(4-adjx)*(4-adjy)) /16;
		if(ex[i].dp_cb)
			ex[i].cb = (df[i-1].cb*Abs(df[i-1].dp_cb)*adjx*(4-adjy)
					+ df[i].cb * Abs(df[i].dp_cb)*(4-adjx)*(4-adjy))
				/ (Abs(df[i-1].dp_cb)*adjx*(4-adjy)+Abs(df[i].dp_cb)*(4-adjx)*(4-adjy));
		// Cr
		ex[i].dp_cr = (df[i-1].dp_cr*adjx*(4-adjy)
							+ df[i].dp_cr*(4-adjx)*(4-adjy)) /16;
		if(ex[i].dp_cr)
			ex[i].cr = (df[i-1].cr*Abs(df[i-1].dp_cr)*adjx*(4-adjy)
					+ df[i].cr * Abs(df[i].dp_cr)*(4-adjx)*(4-adjy))
				/ (Abs(df[i-1].dp_cr)*adjx*(4-adjy)+Abs(df[i].dp_cr)*(4-adjx)*(4-adjy));
	}
	ex[i].dp_y  = df[i-1].dp_y * adjx *(4-adjy)/16;	// �E�[
	ex[i].dp_cb = df[i-1].dp_cb* adjx *(4-adjy)/16;
	ex[i].dp_cr = df[i-1].dp_cr* adjx *(4-adjy)/16;
	ex[i].y  = df[i-1].y;
	ex[i].cb = df[i-1].cb;
	ex[i].cr = df[i-1].cr;

	//----------------------------------------------------------- ��
	for(j=1;j<h-1;j++){
		// Y		//---------------------- ���[
		ex[j*w].dp_y = (df[(j-1)*data->w].dp_y*(4-adjx)*adjy
						+ df[j*data->w].dp_y*(4-adjx)*(4-adjy)) /16;
		if(ex[j*w].dp_y)
			ex[j*w].y = (df[(j-1)*data->w].y*Abs(df[(j-1)*data->w].dp_y)*(4-adjx)*adjy
						+ df[j*data->w].y*Abs(df[j*data->w].dp_y)*(4-adjx)*(4-adjy))
				/ (Abs(df[(j-1)*data->w].dp_y)*(4-adjx)*adjy+Abs(df[j*data->w].dp_y)*(4-adjx)*(4-adjy));
		//Cb
		ex[j*w].dp_cb = (df[(j-1)*data->w].dp_cb*(4-adjx)*adjy
						+ df[j*data->w].dp_cb*(4-adjx)*(4-adjy)) / 16;
		if(ex[j*w].dp_cb)
			ex[j*w].cb = (df[(j-1)*data->w].cb*Abs(df[(j-1)*data->w].dp_cb)*(4-adjx)*adjy
						+ df[j*data->w].cb*Abs(df[j*data->w].dp_cb)*(4-adjx)*(4-adjy))
				/ (Abs(df[(j-1)*data->w].dp_cb)*(4-adjx)*adjy+Abs(df[j*data->w].dp_cb)*(4-adjx)*(4-adjy));
		//Cr
		ex[j*w].dp_cr = (df[(j-1)*data->w].dp_cr*(4-adjx)*adjy
						+ df[j*data->w].dp_cr*(4-adjx)*(4-adjy)) / 16;
		if(ex[j*w].dp_cr)
			ex[j*w].cr = (df[(j-1)*data->w].cr*Abs(df[(j-1)*data->w].dp_cr)*(4-adjx)*adjy
						+ df[j*data->w].cr*Abs(df[j*data->w].dp_cr)*(4-adjx)*(4-adjy))
				/ (Abs(df[(j-1)*data->w].dp_cr)*(4-adjx)*adjy+Abs(df[j*data->w].dp_cr)*(4-adjx)*(4-adjy));
		for(i=1;i<w-1;i++){	//------------------ ��
			// Y
			ex[j*w+i].dp_y = (df[(j-1)*data->w+i-1].dp_y*adjx*adjy
							+ df[(j-1)*data->w+i].dp_y*(4-adjx)*adjy
							+ df[j*data->w+i-1].dp_y*adjx*(4-adjy)
							+ df[j*data->w+i].dp_y*(4-adjx)*(4-adjy) ) /16;
			if(ex[j*w+i].dp_y)
				ex[j*w+i].y = (df[(j-1)*data->w+i-1].y*Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy
							+ df[(j-1)*data->w+i].y*Abs(df[(j-1)*data->w+i].dp_y)*(4-adjx)*adjy
							+ df[j*data->w+i-1].y*Abs(df[j*data->w+i-1].dp_y)*adjx*(4-adjy)
							+ df[j*data->w+i].y*Abs(df[j*data->w+i].dp_y)*(4-adjx)*(4-adjy) )
					/ (Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy + Abs(df[(j-1)*data->w+i].dp_y)*(4-adjx)*adjy
							+ Abs(df[j*data->w+i-1].dp_y)*adjx*(4-adjy)+Abs(df[j*data->w+i].dp_y)*(4-adjx)*(4-adjy));
			// Cb
			ex[j*w+i].dp_cb = (df[(j-1)*data->w+i-1].dp_cb*adjx*adjy
							+ df[(j-1)*data->w+i].dp_cb*(4-adjx)*adjy
							+ df[j*data->w+i-1].dp_cb*adjx*(4-adjy)
							+ df[j*data->w+i].dp_cb*(4-adjx)*(4-adjy) ) /16;
			if(ex[j*w+i].dp_cb)
				ex[j*w+i].cb = (df[(j-1)*data->w+i-1].cb*Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy
							+ df[(j-1)*data->w+i].cb*Abs(df[(j-1)*data->w+i].dp_cb)*(4-adjx)*adjy
							+ df[j*data->w+i-1].cb*Abs(df[j*data->w+i-1].dp_cb)*adjx*(4-adjy)
							+ df[j*data->w+i].cb*Abs(df[j*data->w+i].dp_cb)*(4-adjx)*(4-adjy) )
					/ (Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy + Abs(df[(j-1)*data->w+i].dp_cb)*(4-adjx)*adjy
							+ Abs(df[j*data->w+i-1].dp_cb)*adjx*(4-adjy) + Abs(df[j*data->w+i].dp_cb)*(4-adjx)*(4-adjy));
			// Cr
			ex[j*w+i].dp_cr = (df[(j-1)*data->w+i-1].dp_cr*adjx*adjy
							+ df[(j-1)*data->w+i].dp_cr*(4-adjx)*adjy
							+ df[j*data->w+i-1].dp_cr*adjx*(4-adjy)
							+ df[j*data->w+i].dp_cr*(4-adjx)*(4-adjy) ) /16;
			if(ex[j*w+i].dp_cr)
				ex[j*w+i].cr = (df[(j-1)*data->w+i-1].cr*Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy
							+ df[(j-1)*data->w+i].cr*Abs(df[(j-1)*data->w+i].dp_cr)*(4-adjx)*adjy
							+ df[j*data->w+i-1].cr*Abs(df[j*data->w+i-1].dp_cr)*adjx*(4-adjy)
							+ df[j*data->w+i].cr*Abs(df[j*data->w+i].dp_cr)*(4-adjx)*(4-adjy) )
					/ (Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy +Abs(df[(j-1)*data->w+i].dp_cr)*(4-adjx)*adjy
							+ Abs(df[j*data->w+i-1].dp_cr)*adjx*(4-adjy)+Abs(df[j*data->w+i].dp_cr)*(4-adjx)*(4-adjy));
		}
		// Y		//----------------------- �E�[
		ex[j*w+i].dp_y = (df[(j-1)*data->w+i-1].dp_y*adjx*adjy
						+ df[j*data->w+i-1].dp_y*adjx*(4-adjy)) / 16;
		if(ex[j*w+i].dp_y)
			ex[j*w+i].y = (df[(j-1)*data->w+i-1].y*Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy
						+ df[j*data->w+i-1].y*Abs(df[j*data->w+i-1].dp_y)*adjx*(4-adjy))
				/ (Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy+Abs(df[j*data->w+i-1].dp_y)*adjx*(4-adjy));
		// Cb
		ex[j*w+i].dp_cb = (df[(j-1)*data->w+i-1].dp_cb*adjx*adjy
						+ df[j*data->w+i-1].dp_cb*adjx*(4-adjy)) / 16;
		if(ex[j*w+i].dp_cb)
			ex[j*w+i].cb = (df[(j-1)*data->w+i-1].cb*Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy
						+ df[j*data->w+i-1].cb*Abs(df[j*data->w+i-1].dp_cb)*adjx*(4-adjy))
				/ (Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy+Abs(df[j*data->w+i-1].dp_cb)*adjx*(4-adjy));
		// Cr
		ex[j*w+i].dp_cr = (df[(j-1)*data->w+i-1].dp_cr*adjx*adjy
						+ df[j*data->w+i-1].dp_cr*adjx*(4-adjy)) / 16;
		if(ex[j*w+i].dp_cr)
			ex[j*w+i].cr = (df[(j-1)*data->w+i-1].cr*Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy
						+ df[j*data->w+i-1].cr*Abs(df[j*data->w+i-1].dp_cr)*adjx*(4-adjy))
				/ (Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy+Abs(df[j*data->w+i-1].dp_cr)*adjx*(4-adjy));
	}
	//--------------------------------------------------------- ��ԉ�
	ex[j*w].dp_y  = df[(j-1)*data->w].dp_y *(4-adjx)*adjy /16;	// ���[
	ex[j*w].dp_cb = df[(j-1)*data->w].dp_cb*(4-adjx)*adjy /16;
	ex[j*w].dp_cr = df[(j-1)*data->w].dp_cr*(4-adjx)*adjy /16;
	ex[j*w].y  = df[(j-1)*data->w].y;
	ex[j*w].cb = df[(j-1)*data->w].cb;
	ex[j*w].cr = df[(j-1)*data->w].cr;
	for(i=1;i<w-1;i++){		// ��
		// Y
		ex[j*w+i].dp_y = (df[(j-1)*data->w+i-1].dp_y * adjx * adjy
								+ df[(j-1)*data->w+i].dp_y * (4-adjx) *adjy) /16;
		if(ex[j*w+i].dp_y)
			ex[j*w+i].y = (df[(j-1)*data->w+i-1].y*Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy
						+ df[(j-1)*data->w+i].y*Abs(df[(j-1)*data->w+i].dp_y)*(4-adjx)*adjy)
				/ (Abs(df[(j-1)*data->w+i-1].dp_y)*adjx*adjy +Abs(df[(j-1)*data->w+i].dp_y)*(4-adjx)*adjy);
		// Cb
		ex[j*w+i].dp_cb = (df[(j-1)*data->w+i-1].dp_cb * adjx * adjy
								+ df[(j-1)*data->w+i].dp_cb * (4-adjx) *adjy) /16;
		if(ex[j*w+i].dp_cb)
			ex[j*w+i].cb = (df[(j-1)*data->w+i-1].cb*Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy
						+ df[(j-1)*data->w+i].cb*Abs(df[(j-1)*data->w+i].dp_cb)*(4-adjx)*adjy )
				/ (Abs(df[(j-1)*data->w+i-1].dp_cb)*adjx*adjy +Abs(df[(j-1)*data->w+i].dp_cb)*(4-adjx)*adjy);
		// Cr
		ex[j*w+i].dp_cr = (df[(j-1)*data->w+i-1].dp_cr * adjx * adjy
								+ df[(j-1)*data->w+i].dp_cr * (4-adjx) *adjy) /16;
		if(ex[j*w+i].dp_cr)
			ex[j*w+i].cr = (df[(j-1)*data->w+i-1].cr*Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy
						+ df[(j-1)*data->w+i].cr*Abs(df[(j-1)*data->w+i].dp_cr)*(4-adjx)*adjy)
				/ (Abs(df[(j-1)*data->w+i-1].dp_cr)*adjx*adjy +Abs(df[(j-1)*data->w+i].dp_cr)*(4-adjx)*adjy);
	}
	ex[j*w+i].dp_y  = df[(j-1)*data->w+i-1].dp_y *adjx*adjy /16;	// �E�[
	ex[j*w+i].dp_cb = df[(j-1)*data->w+i-1].dp_cb*adjx*adjy /16;
	ex[j*w+i].dp_cr = df[(j-1)*data->w+i-1].dp_cr*adjx*adjy /16;
	ex[j*w+i].y  = df[(j-1)*data->w+i-1].y;
	ex[j*w+i].cb = df[(j-1)*data->w+i-1].cb;
	ex[j*w+i].cr = df[(j-1)*data->w+i-1].cr;
}


/*--------------------------------------------------------------------
*	update_combobox()		�R���{�{�b�N�X�̑I�����X�V
*		�g���f�[�^�̈�̃��S���ɃR���{�{�b�N�X�̃J�[�\�������킹��
*-------------------------------------------------------------------*/
static void update_cb_logo(char *name)
{
	int num;

	// �R���{�{�b�N�X����
	num = SendMessage(dialog.cb_logo,CB_FINDSTRING,-1,(WPARAM)name);

	if(num == CB_ERR)	// �݂���Ȃ�����
		num = -1;

	SendMessage(dialog.cb_logo,CB_SETCURSEL,num,0);	// �J�[�\���Z�b�g
}

/*--------------------------------------------------------------------
*	change_param()		�p�����[�^�̕ύX
*		�I�����ꂽ���S�f�[�^���g���f�[�^�̈�ɃR�s�[
*-------------------------------------------------------------------*/
static void change_param(FILTER *fp)
{
	LRESULT ret;

	// �I��ԍ��擾
	ret = SendMessage(dialog.cb_logo,CB_GETCURSEL,0,0);
	ret = SendMessage(dialog.cb_logo,CB_GETITEMDATA,ret,0);

	if(ret==CB_ERR) ret = NULL;

	set_exdata(fp,(void *)ret);
}

/*--------------------------------------------------------------------
*	set_cb_logo()
*		�O���t�@�C�����烍�S��ǂݍ��݁A�Z�b�g
*-------------------------------------------------------------------*/
static void set_cb_logo(FILTER *fp)
{
	ULONG   ptr;
	HANDLE  hFile;

	// INI���烍�S�f�[�^�t�@�C������ǂݍ���
	fp->exfunc->ini_load_str(fp,LDP_KEY,logodata_file,NULL);

	if(lstrlen(logodata_file)==0){	// ���S�f�[�^�t�@�C�������Ȃ������Ƃ�
		// �ǂݍ��݃_�C�A���O
		if(!fp->exfunc->dlg_get_load_name(logodata_file,LDP_FILTER,LDP_DEFAULT)){
			// �L�����Z�����ꂽ
			MessageBox(fp->hwnd,"���S�f�[�^�t�@�C��������܂���",filter_name,MB_OK|MB_ICONWARNING);
			return;
		}
	}
	else{	// ���S�f�[�^�t�@�C����������Ƃ�
		// ���݂𒲂ׂ�
		hFile = CreateFile(logodata_file, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

		if(hFile==INVALID_HANDLE_VALUE){	// �݂���Ȃ������Ƃ�
			MessageBox(fp->hwnd,"���S�f�[�^�t�@�C����������܂���",filter_name,MB_OK|MB_ICONWARNING);
			if(!fp->exfunc->dlg_get_load_name(logodata_file,LDP_FILTER,LDP_DEFAULT)){
				// �L�����Z�����ꂽ
				MessageBox(fp->hwnd,"���S�f�[�^�t�@�C��������܂���",filter_name,MB_OK|MB_ICONWARNING);
				return;
			}
		}
		else
			CloseHandle(hFile);
	}

	// ���S�t�@�C���ǂݍ���
	read_logo_pack(logodata_file,fp);

	if(SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0))
		// �g���f�[�^�����l�ݒ�
		fp->ex_data_def = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,0,0);
}

/*--------------------------------------------------------------------
* 	set_combo_item()		�R���{�{�b�N�X�ɃA�C�e�����Z�b�g
* 			data��malloc�Ŋm�ۂ����|�C���^
*-------------------------------------------------------------------*/
static int set_combo_item(void* data)
{
	int num;

	// �R���{�{�b�N�X�A�C�e����
	num = SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0);

	// �Ō���ɒǉ�
	SendMessage(dialog.cb_logo,CB_INSERTSTRING,num,(LPARAM)((LOGO_HEADER *)data)->name);
	SendMessage(dialog.cb_logo,CB_SETITEMDATA,num,(LPARAM)data);

	return num;
}

/*--------------------------------------------------------------------
*	del_combo_item()		�R���{�{�b�N�X����A�C�e�����폜
*-------------------------------------------------------------------*/
static void del_combo_item(int num)
{
	void *ptr;

	ptr = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,num,0);
	if(ptr) free(ptr);

	SendMessage(dialog.cb_logo,CB_DELETESTRING,num,0);
}

/*--------------------------------------------------------------------
* 	read_logo_pack()		���S�f�[�^�ǂݍ���
* 		�t�@�C�����烍�S�f�[�^��ǂݍ���
* 		�R���{�{�b�N�X�ɃZ�b�g
* 		�g���̈�ɃR�s�[
*-------------------------------------------------------------------*/
static void read_logo_pack(char *fname,FILTER *fp)
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
		MessageBox(fp->hwnd,"���S�f�[�^�t�@�C����������܂���",filter_name,MB_OK|MB_ICONERROR);
		return;
	}
	if(GetFileSize(hFile, NULL)<=sizeof(LOGO_HEADER)){	// �T�C�Y�m�F
		CloseHandle(hFile);
		MessageBox(fp->hwnd,"���S�f�[�^�t�@�C�����s���ł�",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	SetFilePointer(hFile,31, 0, FILE_BEGIN);	// �擪����31byte��
	ReadFile(hFile,&num,1,&readed,NULL);	// �f�[�^���擾

	n = 0;	// �������݃f�[�^�J�E���^

	for(i=0;i<num;i++){

		// LOGO_HEADER �ǂݍ���
		readed = 0;
		ReadFile(hFile,&lgh,sizeof(LOGO_HEADER),&readed, NULL);
		if(readed!=sizeof(LOGO_HEADER)){
			MessageBox(fp->hwnd,"���S�f�[�^�̓ǂݍ��݂Ɏ��s���܂���",filter_name,MB_OK|MB_ICONERROR);
			break;
		}

		// �������S�����邩
		same = SendMessage(dialog.cb_logo,CB_FINDSTRING,-1,(WPARAM)lgh.name);
		if(same!=CB_ERR){
			wsprintf(message,"�����̃��S������܂�\n�u�������܂����H\n\n%s",lgh.name);
			if(MessageBox(fp->hwnd,message,filter_name,MB_YESNO|MB_ICONQUESTION)==IDYES){
				// �폜
				del_combo_item(same);
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
			MessageBox(fp->hwnd,"������������܂���",filter_name,MB_OK|MB_ICONERROR);
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

		// �R���{�{�b�N�X�ɒǉ�
		set_combo_item(data);

		n++;
	}

	CloseHandle(hFile);

	if(n){
		// �R���{�{�b�N�X�̃J�[�\���ݒ�
		i = SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0) - n;
		SendMessage(dialog.cb_logo,CB_SETCURSEL,i,0);
		// �g���f�[�^�ݒ�
		set_exdata(fp,(void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,i,0));
	}
}

/*--------------------------------------------------------------------
*	on_option_button()		�I�v�V�����{�^������
*-------------------------------------------------------------------*/
static BOOL on_option_button(FILTER* fp)
{
	LRESULT res;
	void   *data;

	optfp = fp;
	hcb_logo = dialog.cb_logo;

	res = DialogBox(fp->dll_hinst,"OPT_DLG",fp->hwnd,OptDlgProc);

	if(res==IDOK){	// OK�{�^��
		if(SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0)){
			// �A�C�e��������Ƃ�
			data = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,0,0);
			set_exdata(fp,data);
			fp->ex_data_def = data;
		}
	}

	return TRUE;
}

/*--------------------------------------------------------------------
*	set_sended_data()		��M�������S�f�[�^���Z�b�g
*-------------------------------------------------------------------*/
static void set_sended_data(void* logodata,FILTER* fp)
{
	void *ptr;
	UINT same;
	char message[256];
	LOGO_HEADER *lgh;

	lgh = (LOGO_HEADER *)logodata;

	// �����̃��S�����邩�ǂ���
	same = SendMessage(dialog.cb_logo,CB_FINDSTRING,-1,(WPARAM)lgh->name);
	if(same!=CB_ERR){
		wsprintf(message,"�����̃��S������܂�\n�u�������܂����H\n\n%s",logodata);
		if(MessageBox(fp->hwnd,message,filter_name,MB_YESNO|MB_ICONQUESTION)!=IDYES)
			return;	// �㏑�����Ȃ�

		del_combo_item(same);
	}

	ptr = malloc(LOGO_DATASIZE(lgh));
	if(ptr==NULL){
		MessageBox(fp->hwnd,"���������m�ۂł��܂���ł���",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	memcpy(ptr,logodata,LOGO_DATASIZE(logodata));

	set_combo_item(ptr);

	set_exdata(fp,ptr);
}

//*/
