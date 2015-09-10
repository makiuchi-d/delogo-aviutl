/*********************************************************************
* 	���ߐ����S�iBS�}�[�N�Ƃ��j�����t�B���^
* 								ver 0.05
* 
* 2003
* 	02/01:	����J�n
* 	02/02:	�g���f�[�^�̈���g���ƃv���t�@�C���؂�ւ�����AviUtl�������΂�B
* 	      	�Ȃ��H�o�O�H�ǂ����悤�B
* 	      	�Ǝv������A�̈�T�C�Y�ɐ���������̂ˁc SDK�ɂ͈ꌾ�������ĂȂ����ǁB
* 	      	�������|�|!!�i�a�����j������Ɗ����B
* 	      	BS���S���ċP�x�����̕ω��Ȃ̂��HRGB�ł��ƐF�ς�邵�B
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
* 			�v���r���[�Řg����͂ݏo���Ȃ��悤�ɂ����B�i�͂ݏo��������Ɨ�����j
* 	04/20:	�t�B���^���ύX�B���S�t�����[�h���ꎞ�p�~�B
* 	04/28:	1/4�P�ʈʒu���������B
* 			���S�t�����[�h(���܂�Ӗ��Ȃ�����)����
* 			�I�v�V�����_�C�A���O�\������AviUtl���I���ł��Ȃ��悤�ɕύX
* 			�i�G���[���o���ė�����o�O����j
* 
* [������(0.01)���J]
* 
* 	05/04:	�s�����x�����̕��@��ύX�B
* 	05/08:	�������֘A���[�`���啝�ύX		(0.02)
* 			VFAPI����ɑΉ��A�v���t�@�C���̓r���؂�ւ��ɑΉ�
* 			���S�f�[�^�̃T�C�Y�������S�{�ɂ����B
* 	05/10:	�f�[�^����M�ł��Ȃ��Ȃ��Ă����o�O���C��	(0.03)
* 	05/17:	���S����ҏW�ł���悤�ɂ����B(0.04)
* 	06/12:	�v���r���[�̔w�i�F��RGB�Ŏw��ł���悤�ɕύX�B
* 			�ʒu�������S�̔{���̂Ƃ�create_adj_exdata()���Ă΂Ȃ��悤�ɂ����B�i0.05�j
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
* 	�E���S�ɂP�s�N�Z�������̃Y��������B1/4�s�N�Z�����炢�ł̈ʒu�������K�v�����B
* 		����������
* 	�E�_�C�A���O��\�������܂܏I������ƃG���[�f��
* 		���e�E�B���h�E��AviUtl�{�̂ɂ��邱�ƂŏI���ł��Ȃ�����
* 	�E���S�f�[�^�\�������ς��悤���ȁc �F���v�f�̃r�b�g�𔼕��ɂ���Ƃ��Bver0.10�ł�邩�H
* 
*  �V�������Ǘ��ɂ���:(2003/05/08)
* 	fp->ex_data_ptr�ɂ̓��S�̖��݂̂̂�ۑ��B�i7FFD�o�C�g�����v���t�@�C���ɕۑ����ꂸ�A�s��������邽�߁j
* 	func_init()�Ń��S�f�[�^�p�b�N��ǂݍ��ށBldp�t�@�C�����͕K���t���p�X�ł��邱�Ƃ��K�v�B
* 	�ǂݍ��񂾃��S�f�[�^�̃|�C���^��logodata�z��ɕۑ��B�z��̃f�[�^����logodata_n�ɁB
* 	func_proc()�ł�ex_data�i���S���́j�ƈ�v����f�[�^��logodata�z�񂩂猟���B�Ȃ������ꍇ�͉������Ȃ��B
* 	�ʒu�p�����[�^���g���Ĉʒu�����f�[�^���쐬�B���̌�ŏ����E�t���֐����ĂԁB
* 	WndProc�ł́AWM_FILTER_INIT�ŃR���{�{�b�N�X�A�C�e����logodata�z�񂩂���B
* 	�R���{�A�C�e����ITEMDATA�ɂ͏]���ǂ��胍�S�f�[�^�̃|�C���^��ۑ�����B
* 	WM_FILTER_INIT�ł̓R���{�{�b�N�X�A�C�e������t�@�C���ɕۑ��B�i���܂łǂ���j
* 	�I�v�V�����ݒ�_�C�A���O�ł̃��S�f�[�^�̓ǂݍ��݁E�폜�͍��܂łǂ���B
* 	OK�{�^���������ꂽ�Ƃ��́A���X�g�A�C�e������logodata�z�����蒼���B�R���{�A�C�e���̍X�V�͍��܂łǂ���B
* 
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
#define LDP_FILTER  "���S�f�[�^�p�b�N (*.ldp)\0*.ldp\0"\
                    "AllFiles (*.*)\0*.*\0"


// �_�C�A���O�A�C�e��
typedef struct {
	HFONT font;
	HWND  cb_logo;
	HWND  bt_opt;
} FILTER_DIALOG;

FILTER_DIALOG dialog;

char  logodata_file[MAX_PATH] = { 0 };	// ���S�f�[�^�t�@�C����(INI�ɕۑ�)
UINT  WM_SEND_LOGO_DATA;

LOGO_HEADER** logodata   = NULL;
unsigned int  logodata_n = 0;

char ex_data[LOGO_MAX_NAME];	// �g���f�[�^�̈�

static UINT  WM_SEND_LOGO_DATA;	// ���S��M���b�Z�[�W


//----------------------------
//	�v���g�^�C�v�錾
//----------------------------
static void on_wm_filter_init(FILTER* fp);
static void on_wm_filter_exit(FILTER* fp);
static void init_dialog(HWND hwnd,HINSTANCE hinst);
static void update_cb_logo(char *name);
static void change_param(void);
static void set_cb_logo(FILTER* fp);
static int  set_combo_item(void* data);
static void del_combo_item(int num);
static void read_logo_pack(char *logodata_file,FILTER *fp);
static void set_sended_data(void* logodata,FILTER* fp);
static BOOL create_adj_exdata(FILTER *fp,LOGO_HEADER *adjdata,const LOGO_HEADER *data);
static int  find_logo(const char *logo_name);

static BOOL on_option_button(FILTER* fp);

BOOL func_proc_eraze_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh);
BOOL func_proc_add_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh);

//----------------------------
//	FILTER_DLL�\����
//----------------------------
char filter_name[] = LOGO_FILTER_NAME;
char filter_info[] = LOGO_FILTER_NAME" ver 0.05 by MakKi";
#define track_N 6
#if track_N
TCHAR *track_name[]   = { "�ʒu X", "�ʒu Y", "�[�x", "Y", "Cb", "Cr" };	// �g���b�N�o�[�̖��O
int   track_default[] = {   0,   0,   100,   0,   0,   0 };	// �g���b�N�o�[�̏����l
int   track_s[]       = { -200, -200,   0, -100, -100, -100 };	// �g���b�N�o�[�̉����l
int   track_e[]       = {  200,  200, 200,  100,  100,  100 };	// �g���b�N�o�[�̏���l
#endif
#define check_N 2
#if check_N
TCHAR *check_name[]   = { "���S�t�����[�h","���S�������[�h" };	// �`�F�b�N�{�b�N�X
int   check_default[] = { 0, 1 };	// �f�t�H���g
#endif


#define LOGO_X      0
#define LOGO_Y      1
#define LOGO_YDP    2
#define LOGO_CBDP   2
#define LOGO_CRDP   2
#define LOGO_PY     3
#define LOGO_CB     4
#define LOGO_CR     5

// �ݒ�E�B���h�E�̍���
#define WND_Y (67+24*track_N+20*check_N)


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
	func_init,		// �J�n���ɌĂ΂��
	func_exit,   	// �I�����ɌĂ΂��֐�
	NULL,        	// �ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐�
	func_WndProc,	// �ݒ�E�B���h�E�v���V�[�W��
	NULL,NULL,   	// �V�X�e���Ŏg�p
	ex_data,     	// �g���f�[�^�̈�
	sizeof(LOGO_HEADER),//57102,	// �g���f�[�^�T�C�Y
	filter_info, 	// �t�B���^���
	NULL,			// �Z�[�u�J�n���O�ɌĂ΂��֐�
	NULL,			// �Z�[�u�I�����ɌĂ΂��֐�
	NULL,NULL,NULL,	// �V�X�e���Ŏg�p
	ex_data,		// �g���̈揉���l
};




/*********************************************************************
*	DLL Export
*********************************************************************/
EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable( void )
{
	return &filter;
}

/*====================================================================
*	�J�n���ɌĂ΂��֐�
*===================================================================*/
BOOL func_init( FILTER *fp )
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
			return FALSE;
		}
	}
	else{	// ���S�f�[�^�t�@�C����������Ƃ�
		// ���݂𒲂ׂ�
		hFile = CreateFile(logodata_file, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

		if(hFile==INVALID_HANDLE_VALUE){	// �݂���Ȃ������Ƃ�
			MessageBox(fp->hwnd,"���S�f�[�^�t�@�C����������܂���",filter_name,MB_OK|MB_ICONWARNING);
			if(!fp->exfunc->dlg_get_load_name(logodata_file,LDP_FILTER,LDP_DEFAULT)){
				// �L�����Z�����ꂽ
				MessageBox(fp->hwnd,"���S�f�[�^�t�@�C�����w�肳��Ă��܂���",filter_name,MB_OK|MB_ICONWARNING);
				return FALSE;
			}
		}
		else
			CloseHandle(hFile);
	}

	// ���S�t�@�C���ǂݍ���
	read_logo_pack(logodata_file,fp);

	if(logodata_n)
		// �g���f�[�^�����l�ݒ�
		fp->ex_data_def = logodata[0];

	return TRUE;
}

/*====================================================================
*	�I�����ɌĂ΂��֐�
*===================================================================*/
BOOL func_exit( FILTER *fp )
{
	unsigned int i;

	// ���S�f�[�^�J��
	if(logodata){
		for(i=0;i<logodata_n;i++){
			if(logodata[i]){
				free(logodata[i]);
				logodata[i] = NULL;
			}
		}
		free(logodata);
		logodata = NULL;
	}

	return TRUE;
}

/*====================================================================
*	�t�B���^�����֐�
*===================================================================*/
BOOL func_proc(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	int num;
	char adjdata[LOGO_MAXSIZE];

	// ���S����
	num = find_logo(fp->ex_data_ptr);
	if(num<0) return FALSE;

	if(fp->track[LOGO_X]%4 || fp->track[LOGO_Y]%4){
		// �ʒu�������S�̔{���łȂ��Ƃ��A1/4�s�N�Z���P�ʒ���
		if(!create_adj_exdata(fp,(void *)adjdata,logodata[num]))
			return FALSE;
	}
	else {
		// 4�̔{���̂Ƃ���x,y�̂ݏ�������
		memcpy(adjdata,logodata[num],LOGO_DATASIZE(logodata[num]));
		((LOGO_HEADER *)adjdata)->x += fp->track[LOGO_X] / 4;
		((LOGO_HEADER *)adjdata)->y += fp->track[LOGO_Y] / 4;
	}

	if(fp->check[1])	// �������[�h�`�F�b�N
		return func_proc_eraze_logo(fp,fpip,(void *)adjdata);	// ���S�������[�h
	else
		return func_proc_add_logo(fp,fpip,(void *)adjdata);		// ���S�t�����[�h
}

/*--------------------------------------------------------------------
* 	func_proc_eraze_logo()	���S�������[�h
*-------------------------------------------------------------------*/
BOOL func_proc_eraze_logo(FILTER* fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh)
{
	int   i,j;
	int   y,cb,cr;
	PIXEL_YC    *ptr;
	LOGO_PIXEL  *lgp;
	double dp,temp;


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
				dp = lgp->dp_y * ((double)fp->track[LOGO_YDP]/100);	// ����
				y  = lgp->y + fp->track[LOGO_PY]*16;
				if(dp==LOGO_MAX_DP) dp--;	// 0�ł̏��Z���
				temp = ((double)ptr->y*LOGO_MAX_DP - y*dp) / (LOGO_MAX_DP - dp) +0.5;	// �t�Z
				if  (temp>4096) temp = 4096;	// �͈̓`�F�b�N
				else if(temp<0) temp = 0;
				ptr->y = temp;

				// �F��(��)
				dp = lgp->dp_cb * ((double)fp->track[LOGO_CBDP]/100);
				cb = lgp->cb    + fp->track[LOGO_CB]*16;
				if(dp==LOGO_MAX_DP) dp--;	// 0�ł̏��Z���
				temp = ((double)ptr->cb*LOGO_MAX_DP - cb*dp) / (LOGO_MAX_DP - dp) +0.5;
				if      (temp>2048) temp =  2048;	// �͈̓`�F�b�N
				else if(temp<-2048) temp = -2048;
				ptr->cb = temp;

				// �F��(��)
				dp = lgp->dp_cr * ((double)fp->track[LOGO_CRDP]/100);
				cr = lgp->cr   + fp->track[LOGO_CR]*16;
				if(dp==LOGO_MAX_DP) dp--;	// 0�ł̏��Z���
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
BOOL func_proc_add_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh)
{
	int   i,j;
	int   y,cb,cr;
	PIXEL_YC    *ptr;
	LOGO_PIXEL  *lgp;
	double dp,temp;


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
				dp = lgp->dp_y * ((double)fp->track[LOGO_YDP]/100);
				y  = lgp->y    + fp->track[LOGO_PY]*16;
				temp  = ((double)ptr->y*(LOGO_MAX_DP-dp) + y*dp) / LOGO_MAX_DP +0.5;	// ���S�t��
				if  (temp>4096) temp = 4096;	// �͈̓`�F�b�N
				else if(temp<0) temp = 0;
				ptr->y = temp;


				// �F��(��)
				dp = lgp->dp_cb * ((double)fp->track[LOGO_CBDP]/100);
				cb = lgp->cb    + fp->track[LOGO_CB]*16;
				temp  = ((double)ptr->cb*(LOGO_MAX_DP-dp) + cb*dp) / LOGO_MAX_DP +0.5;
				if     (temp> 2048) temp =  2048;	// �͈̓`�F�b�N
				else if(temp<-2048) temp = -2048;
				ptr->cb = temp;

				// �F��(��)			//pow(2,(double)fp->track[LOGO_CRDP]/128);
				dp = lgp->dp_cr * ((double)fp->track[LOGO_CRDP]/100);
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

/*--------------------------------------------------------------------
* 	find_logo()		���S�����烍�S�f�[�^������
*-------------------------------------------------------------------*/
static int  find_logo(const char *logo_name)
{
	unsigned int i;

	for(i=0;i<logodata_n;i++){
		if(lstrcmp((char *)logodata[i],logo_name)==0)
			return i;
	}

	return -1;
}

/*====================================================================
*	�ݒ�E�B���h�E�v���V�[�W��
*===================================================================*/
BOOL func_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void *editp, FILTER *fp )
{
	if(message==WM_SEND_LOGO_DATA){	// ���S�f�[�^��M
		set_sended_data((void *)wParam,fp);
		return TRUE;
	}

	switch(message){
		case WM_FILTER_INIT:	// ������
			on_wm_filter_init(fp);
			return TRUE;

		case WM_FILTER_EXIT:	// �I��
			on_wm_filter_exit(fp);
			break;

		case WM_FILTER_UPDATE:	// �t�B���^�X�V
		case WM_FILTER_SAVE_END:	// �Z�[�u�I��
			// �R���{�{�b�N�X�\���X�V
			update_cb_logo(ex_data);
			break;

		//---------------------------------------------�{�^������
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case ID_BUTTON_OPTION:	// �I�v�V�����{�^��
					return on_option_button(fp);

				case ID_COMBO_LOGO:	// �R���{�{�b�N�X
					switch(HIWORD(wParam)){
						case CBN_SELCHANGE:	// �I��ύX
							change_param();
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
* 	on_wm_filter_init()		�ݒ�E�B���h�E������
*-------------------------------------------------------------------*/
static void on_wm_filter_init(FILTER* fp)
{
	unsigned int i;

	init_dialog(fp->hwnd,fp->dll_hinst);
	// �R���{�A�C�e���Z�b�g
	for(i=0;i<logodata_n;i++)
		set_combo_item(logodata[i]);

	// ���S�f�[�^��M���b�Z�[�W�o�^
	WM_SEND_LOGO_DATA = RegisterWindowMessage(wm_send_logo_data);
}

/*--------------------------------------------------------------------
* 	on_wm_filter_exit()		�I������
* 		�ǂݍ��܂�Ă��郍�S�f�[�^��ldp�ɕۑ�
*-------------------------------------------------------------------*/
static void on_wm_filter_exit(FILTER* fp)
{
	int    i,num;
	char   n;
	DWORD  dw;
	HANDLE hFile;
	void*  data;

	if(lstrlen(logodata_file)==0){	// ���S�f�[�^�t�@�C�������Ȃ��Ƃ�
		if(!fp->exfunc->dlg_get_load_name(logodata_file,LDP_FILTER,LDP_DEFAULT)){
			// �L�����Z�����ꂽ
			MessageBox(fp->hwnd,"���S�f�[�^�͕ۑ�����܂���",filter_name,MB_OK|MB_ICONWARNING);
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
				MessageBox(fp->hwnd,"���S�f�[�^�͕ۑ�����܂���",filter_name,MB_OK|MB_ICONWARNING);
				return;
			}
		}
		else
			CloseHandle(hFile);
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
*	create_adj_exdata()		�ʒu�������S�f�[�^�쐬
*-------------------------------------------------------------------*/
static BOOL create_adj_exdata(FILTER *fp,LOGO_HEADER *adjdata,const LOGO_HEADER *data)
{
	int  i,j;
	int  w,h;
	int  temp;
	int  adjx,adjy;
	LOGO_PIXEL *df;
	LOGO_PIXEL *ex;

	if(data == NULL)
		return FALSE;

	// ���S���R�s�[
	memcpy(adjdata->name,data->name,LOGO_MAX_NAME);

	// ������W�ݒ�i�ʒu������j
	adjdata->x = data->x +(int)(fp->track[LOGO_X]+200)/4 -50;
	adjdata->y = data->y +(int)(fp->track[LOGO_Y]+200)/4 -50;

	adjdata->w = w = data->w + 1;	// 1/4�P�ʒ������邽��
	adjdata->h = h = data->h + 1;	// ���A�������P���₷

	// LOGO_PIXEL�̐擪
	(void *)df = (void *)(data +1);
	(void *)ex = (void *)(adjdata +1);

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
		// �P�xY		//---------------------- ���[
		ex[j*w].dp_y = (df[(j-1)*data->w].dp_y*(4-adjx)*adjy
						+ df[j*data->w].dp_y*(4-adjx)*(4-adjy)) /16;
		if(ex[j*w].dp_y)
			ex[j*w].y = (df[(j-1)*data->w].y*Abs(df[(j-1)*data->w].dp_y)*(4-adjx)*adjy
						+ df[j*data->w].y*Abs(df[j*data->w].dp_y)*(4-adjx)*(4-adjy))
				/ (Abs(df[(j-1)*data->w].dp_y)*(4-adjx)*adjy+Abs(df[j*data->w].dp_y)*(4-adjx)*(4-adjy));
		// �F��(��)Cb
		ex[j*w].dp_cb = (df[(j-1)*data->w].dp_cb*(4-adjx)*adjy
						+ df[j*data->w].dp_cb*(4-adjx)*(4-adjy)) / 16;
		if(ex[j*w].dp_cb)
			ex[j*w].cb = (df[(j-1)*data->w].cb*Abs(df[(j-1)*data->w].dp_cb)*(4-adjx)*adjy
						+ df[j*data->w].cb*Abs(df[j*data->w].dp_cb)*(4-adjx)*(4-adjy))
				/ (Abs(df[(j-1)*data->w].dp_cb)*(4-adjx)*adjy+Abs(df[j*data->w].dp_cb)*(4-adjx)*(4-adjy));
		// �F��(��)Cr
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

	return TRUE;
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
static void change_param(void)
{
	LRESULT ret;

	// �I��ԍ��擾
	ret = SendMessage(dialog.cb_logo,CB_GETCURSEL,0,0);
	ret = SendMessage(dialog.cb_logo,CB_GETITEMDATA,ret,0);

	if(ret!=CB_ERR)
		memcpy(ex_data,(void *)ret,LOGO_MAX_NAME);	// ���S�����R�s�[
}

/*--------------------------------------------------------------------
* 	set_combo_item()		�R���{�{�b�N�X�ɃA�C�e�����Z�b�g
* 			data��malloc�Ŋm�ۂ����|�C���^
* 			�ʂ�logodata�z��ɏ������ޕK�v������
*-------------------------------------------------------------------*/
static int set_combo_item(void* data)
{
	int num;

	// �R���{�{�b�N�X�A�C�e����
	num = SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0);

	// �Ō���ɒǉ�
	SendMessage(dialog.cb_logo,CB_INSERTSTRING,num,(LPARAM)data);
	SendMessage(dialog.cb_logo,CB_SETITEMDATA,num,(LPARAM)data);

	return num;
}

/*--------------------------------------------------------------------
*	del_combo_item()		�R���{�{�b�N�X����A�C�e�����폜
*-------------------------------------------------------------------*/
static void del_combo_item(int num)
{
	void *ptr;
	unsigned int i;

	ptr = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,num,0);
	if(ptr) free(ptr);

	// ���S�f�[�^�z��č\��
	logodata_n = SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0);
	logodata = realloc(logodata,logodata_n*sizeof(logodata));

	for(i=0;i<logodata_n;i++)
		logodata[i] = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,i,0);

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
	int i;
	int same;
	char message[255];

	// �t�@�C���I�[�v��
	hFile = CreateFile(fname,GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile==INVALID_HANDLE_VALUE){
		MessageBox(fp->hwnd,"���S�f�[�^�t�@�C����������܂���",filter_name,MB_OK|MB_ICONERROR);
		return;
	}
	if(GetFileSize(hFile, NULL)<sizeof(LOGO_HEADER)){	// �T�C�Y�m�F
		CloseHandle(hFile);
		MessageBox(fp->hwnd,"���S�f�[�^�t�@�C�����s���ł�",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	SetFilePointer(hFile,31, 0, FILE_BEGIN);	// �擪����31byte��
	ReadFile(hFile,&num,1,&readed,NULL);	// �f�[�^���擾

	logodata_n = 0;	// �������݃f�[�^�J�E���^
	logodata = NULL;

	for(i=0;i<num;i++){

		// LOGO_HEADER �ǂݍ���
		readed = 0;
		ReadFile(hFile,&lgh,sizeof(LOGO_HEADER),&readed, NULL);
		if(readed!=sizeof(LOGO_HEADER)){
			MessageBox(fp->hwnd,"���S�f�[�^�̓ǂݍ��݂Ɏ��s���܂���",filter_name,MB_OK|MB_ICONERROR);
			break;
		}

//  ldp�ɂ͊�{�I�ɓ����̃��S�͑��݂��Ȃ�
//
//		// �������S�����邩
//		same = find_logodata(lgh.name);
//		if(same>0){
//			wsprintf(message,"�����̃��S������܂�\n�u�������܂����H\n\n%s",lgh.name);
//			if(MessageBox(fp->hwnd,message,filter_name,MB_YESNO|MB_ICONQUESTION)==IDYES){
//				// �폜
//				del_combo_item(same);
//			}
//			else{	// �㏑�����Ȃ�
//				// �t�@�C���|�C���^��i�߂�
//				SetFilePointer(hFile,LOGO_PIXELSIZE(&lgh), 0, FILE_CURRENT);
//				continue;
//			}
//		}

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

		// logodata�|�C���^�z��ɒǉ�
		logodata_n++;
		logodata = realloc(logodata,logodata_n*sizeof(logodata));
		logodata[logodata_n-1] = data;
	}

	CloseHandle(hFile);

	if(logodata_n){
		// �g���f�[�^�ݒ�
		memcpy(ex_data,logodata[0],LOGO_MAX_NAME);
	}
}

/*--------------------------------------------------------------------
*	on_option_button()		�I�v�V�����{�^������
*-------------------------------------------------------------------*/
static BOOL on_option_button(FILTER* fp)
{
	LRESULT res;
	void   *data;
	unsigned int i;

	// �I�v�V�����_�C�A���O���Q�Ƃ���
	optfp = fp;
	hcb_logo = dialog.cb_logo;

	EnableWindow(dialog.bt_opt,FALSE);	// �I�v�V�����{�^��������

	// �I�v�V�����_�C�A���O�\���i���[�_���t���[���j
	res = DialogBox(fp->dll_hinst,"OPT_DLG",GetWindow(fp->hwnd, GW_OWNER),OptDlgProc);

	EnableWindow(dialog.bt_opt,TRUE);	// �L���ɖ߂�

	if(res==IDOK){	// OK�{�^��
		logodata_n = SendMessage(dialog.cb_logo,CB_GETCOUNT,0,0);

		// logodata�z��č\��
		logodata = realloc(logodata,logodata_n*sizeof(logodata));
		for(i=0;i<logodata_n;i++)
			logodata[i] = (void *)SendMessage(dialog.cb_logo,CB_GETITEMDATA,i,0);

		if(logodata_n)	// �g���f�[�^�����l�ݒ�
			fp->ex_data_def = logodata[0];
		else
			fp->ex_data_def = NULL;
	}

	return TRUE;
}

/*--------------------------------------------------------------------
*	set_sended_data()		��M�������S�f�[�^���Z�b�g
*-------------------------------------------------------------------*/
static void set_sended_data(void* data,FILTER* fp)
{
	void *ptr;
	UINT same;
	char message[256];
	LOGO_HEADER *lgh;

	lgh = (LOGO_HEADER *)data;

	// �����̃��S�����邩�ǂ���
	same = SendMessage(dialog.cb_logo,CB_FINDSTRING,-1,(WPARAM)lgh->name);
	if(same!=CB_ERR){
		wsprintf(message,"�����̃��S������܂�\n�u�������܂����H\n\n%s",data);
		if(MessageBox(fp->hwnd,message,filter_name,MB_YESNO|MB_ICONQUESTION)!=IDYES)
			return;	// �㏑�����Ȃ�

		del_combo_item(same);
	}

	ptr = malloc(LOGO_DATASIZE(lgh));
	if(ptr==NULL){
		MessageBox(fp->hwnd,"���������m�ۂł��܂���ł���",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

	memcpy(ptr,data,LOGO_DATASIZE(data));

	logodata_n++;
	logodata = realloc(logodata,logodata_n*sizeof(logodata));

	logodata[logodata_n-1] = ptr;
	set_combo_item(ptr);
}

//*/
