/*********************************************************************
* 	���ߐ����S�iBS�}�[�N�Ƃ��j�����t�B���^
* 								ver 0.11
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
* 	06/30:	�t�F�[�h�C���E�A�E�g�ɑΉ��B (0.06)
* 	07/02:	���S�f�[�^����M�ł��Ȃ��ꍇ���������̂��C���B
* 	07/03:	YCbCr�͈̔̓`�F�b�N������悤�ɂ����B(���Ȃ��Ɨ����邱�Ƃ�����)
* 			���S���ҏW�œ����ɂ����Ă��ł��Ȃ��悤�ɂ����B(0.06a)
* 	08/01:	�t�F�[�h�̕s�����x�v�Z����������
* 	08/02:	�������Z���~�߁A���ʂȉ��Z���폜���č������B
* 			��ɔ����[�x�̃f�t�H���g�l��ύX�B
* 			�ׂ��ȏC��
* 	09/05:	�ׂ��ȏC��
* 	09/27:	filter.h��AviUtl0.99SDK�̂��̂ɍ����ւ��B(0.07)
* 	10/20:	SSE2�g�p��rgb2yc���o�O�����Ȃ̂ŁA���O��RGB->YCbCr����悤�ɂ����B
* 			�ʒuX/Y�̍ő奍ŏ��l���g�������B(0.07a)
* 	10/25:	�ʒu������-200�ȉ��ɂ���Ɨ�����o�O�C���B(0.07b)
* 2004
* 	02/18:	AviSynth�X�N���v�g��f���{�^���ǉ��B(0.08)
* 	04/17:	���S�f�[�^�t�@�C���ǂݍ��ݎ��Ƀf�[�^������������G���[���o���Ȃ��悤�ɂ����B
* 			�J�n��I���̍ő�l��4096�܂ő��₵���B(0.08a)
* 	09/19:	�X�^�b�N�𖳑ʌ������Ă����̂��C���B
* 			�J�n�E�t�F�[�h�C���E�A�E�g�E�I���̏����l�����S�f�[�^�ɕۑ��ł���悤�ɂ����B(0.09)
* 2005
* 	04/18:	�t�B���^���A�p�����^����ύX�ł���悤�ɂ����B(0.09a)
* 2007
* 	11/07:	�v���t�@�C���̋��E���t�F�[�h�̊�_�ɂł���悤�ɂ����B(0.10)
* 2008
*	01/07:	���S�̃T�C�Y������P�p
*			�J�n�E�I���p�����^�͈͕̔ύX(���̒l������)
*			���S�t�@�C���̃f�[�^�����g��(1byte -> 4byte)
* 
*********************************************************************/

/* ToDo:
* 	�E���S�f�[�^�̍쐬�E�ҏW�@�\
* 
*  MEMO:
* 	�E���S�̊g��k�����[�`���������Ȃ��Ƃ��߂��Ȃ��B
* 		���K�v�Ȃ����B���ʂ͎������Ȃ��B
* 	�E���S�쐬�E�ҏW�͕ʃA�v���ɂ��Ă��܂������c
* 		�d�l���J���Ă邵�A�N������Ă���Ȃ����Ȃ��i���͖{��j
* 	�E���S�������[�h�ƃ��S�t�����[�h��؂�ւ�����悤�ɂ��悤����
* 		���t���Ă݂�
* 	�E��̓v���O����f�[�^���󂯎��ɂ́c�Ǝ�WndMsg�o�^����SendMessage�ő����Ă��炤
* 		�������Ɠ������B�c�o�^���Ȃ��Ă�USER��`�ł悩��������
* 	�E���S�ɂP�s�N�Z�������̃Y��������B1/4�s�N�Z�����炢�ł̈ʒu�������K�v�����B
* 		����������
* 	�E�_�C�A���O��\�������܂܏I������ƃG���[�f��
* 		���e�E�B���h�E��AviUtl�{�̂ɂ��邱�ƂŏI���ł��Ȃ�����
* 	�E���S�f�[�^�\�������ς��悤���ȁc �F���v�f�̃r�b�g�𔼕��ɂ���Ƃ��B
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
#include "strdlg.h"
#include "logodef.h"


#define ID_BUTTON_OPTION 40001
#define ID_COMBO_LOGO    40002
#define ID_BUTTON_SYNTH  40003

#define Abs(x) ((x>0)? x:-x)
#define Clamp(n,l,h) ((n<l) ? l : (n>h) ? h : n)


#define LDP_KEY     "logofile"
#define LDP_DEFAULT "logodata.ldp"
#define LDP_FILTER  "���S�f�[�^�p�b�N (*.ldp)\0*.ldp\0"\
                    "AllFiles (*.*)\0*.*\0"


// �_�C�A���O�A�C�e��
typedef struct {
	HFONT font;
	HWND  cb_logo;
	HWND  bt_opt;
	HWND  bt_synth;
} FILTER_DIALOG;

FILTER_DIALOG dialog;

char  logodata_file[MAX_PATH] = { 0 };	// ���S�f�[�^�t�@�C����(INI�ɕۑ�)

LOGO_HEADER** logodata   = NULL;
unsigned int  logodata_n = 0;

void *adjdata = NULL;	// �ʒu���ߌネ�S�f�[�^�p�o�b�t�@
unsigned int adjdata_size = 0;

char ex_data[LOGO_MAX_NAME];	// �g���f�[�^�̈�

static UINT  WM_SEND_LOGO_DATA =0;	// ���S��M���b�Z�[�W


//----------------------------
//	�v���g�^�C�v�錾
//----------------------------
static void on_wm_filter_init(FILTER* fp);
static void on_wm_filter_exit(FILTER* fp);
static void init_dialog(HWND hwnd,HINSTANCE hinst);
static void update_cb_logo(char *name);
static void change_param(FILTER* fp);
static void set_cb_logo(FILTER* fp);
static int  set_combo_item(void* data);
static void del_combo_item(int num);
static void read_logo_pack(char *logodata_file,FILTER *fp);
static void set_sended_data(void* logodata,FILTER* fp);
static BOOL create_adj_exdata(FILTER *fp,LOGO_HEADER *adjdata,const LOGO_HEADER *data);
static int  find_logo(const char *logo_name);
static int calc_fade(FILTER *fp,FILTER_PROC_INFO *fpip);

static BOOL on_option_button(FILTER* fp);
static BOOL on_avisynth_button(FILTER* fp,void* editp);

BOOL func_proc_eraze_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh,int);
BOOL func_proc_add_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh,int);

//----------------------------
//	FILTER_DLL�\����
//----------------------------
char filter_name[] = LOGO_FILTER_NAME;
char filter_info[] = LOGO_FILTER_NAME" ver 0.11 by MakKi";
#define track_N 10
#if track_N
TCHAR *track_name[]   = { 	"�ʒu X", "�ʒu Y", 
							"�[�x", "Y", "Cb", "Cr", 
							"�J�n", "FadeIn", "FadeOut", "�I��" };	// �g���b�N�o�[�̖��O
int   track_default[] = { 0, 0,
						  128, 0, 0, 0,
						  0, 0, 0, 0, 0 };	// �g���b�N�o�[�̏����l
int   track_s[] = { LOGO_XY_MIN, LOGO_XY_MIN,
					0, -100, -100, -100,
					LOGO_STED_MIN, 0, 0, LOGO_STED_MIN };	// �g���b�N�o�[�̉����l
int   track_e[] = { LOGO_XY_MAX, LOGO_XY_MAX,
					256, 100, 100, 100,
					LOGO_STED_MAX, LOGO_FADE_MAX, LOGO_FADE_MAX, LOGO_STED_MAX };	// �g���b�N�o�[�̏���l
#endif
#define check_N 3
#if check_N
TCHAR *check_name[]   = { "���S�t�����[�h","���S�������[�h","���̧�ً��E��̪��ފ�_�ɂ���" };	// �`�F�b�N�{�b�N�X
int   check_default[] = { 0, 1, 0 };	// �f�t�H���g
#endif

#define LOGO_X      0
#define LOGO_Y      1
#define LOGO_YDP    2
#define LOGO_CBDP   2
#define LOGO_CRDP   2
#define LOGO_PY     3
#define LOGO_CB     4
#define LOGO_CR     5
#define LOGO_STRT   6
#define LOGO_FIN    7
#define LOGO_FOUT   8
#define LOGO_END    9

#define LOGO_ADDMODE 0
#define LOGO_DELMODE 1
#define LOGO_BASEPROFILE 2

// �ݒ�E�B���h�E�̍���
#define WND_Y (67+24*track_N+20*check_N)


FILTER_DLL filter = {
	FILTER_FLAG_WINDOW_SIZE |	//	�t�B���^�̃t���O
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

	free(adjdata);
	adjdata = NULL;
	adjdata_size = 0;

	return TRUE;
}

/*====================================================================
*	�t�B���^�����֐�
*===================================================================*/
BOOL func_proc(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	unsigned int size;
	int num;
	int fade;

	// ���S����
	num = find_logo(fp->ex_data_ptr);
	if(num<0) return FALSE;

	size = sizeof(LOGO_HEADER)
		+ (logodata[num]->h+1) * (logodata[num]->w+1) * sizeof(LOGO_PIXEL);
	if(size > adjdata_size){
		adjdata = realloc(adjdata,size);
		adjdata_size = size;
	}
	if(adjdata==NULL){	//�m�ێ��s
		adjdata_size = 0;
		return FALSE;
	}

	fade = calc_fade(fp,fpip);

	if(fp->track[LOGO_X]%4 || fp->track[LOGO_Y]%4){
		// �ʒu�������S�̔{���łȂ��Ƃ��A1/4�s�N�Z���P�ʒ���
		if(!create_adj_exdata(fp,(void *)adjdata,logodata[num]))
			return FALSE;
	}
	else{
		// 4�̔{���̂Ƃ���x,y�̂ݏ�������
		memcpy(adjdata,logodata[num],size);
		((LOGO_HEADER *)adjdata)->x += fp->track[LOGO_X] / 4;
		((LOGO_HEADER *)adjdata)->y += fp->track[LOGO_Y] / 4;
	}

	if(fp->check[LOGO_DELMODE])	// �������[�h�`�F�b�N
		return func_proc_eraze_logo(fp,fpip,(void *)adjdata,fade);	// ���S�������[�h
	else
		return func_proc_add_logo(fp,fpip,(void *)adjdata,fade);	// ���S�t�����[�h
}

/*--------------------------------------------------------------------
* 	func_proc_eraze_logo()	���S�������[�h
*-------------------------------------------------------------------*/
BOOL func_proc_eraze_logo(FILTER* fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh,int fade)
{
	int   i,j;
	int   yc;
	int   dp;
	PIXEL_YC    *ptr;
	LOGO_PIXEL  *lgp;


	// LOGO_PIXEL�f�[�^�ւ̃|�C���^
	(void *)lgp = lgh + 1;

	// ����̈ʒu�ֈړ�
	ptr = fpip->ycp_edit;
	ptr += lgh->x + lgh->y * fpip->max_w;

	for(i=0;i < lgh->h;++i){
		for(j=0;j < lgh->w;++j){

			if(ptr >= fpip->ycp_edit &&	// ��ʓ��̎��̂ݏ���
			   ptr < fpip->ycp_edit + fpip->max_w*fpip->h)
			{
				// �P�x
				dp = (lgp->dp_y * fp->track[LOGO_YDP] * fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					if(dp==LOGO_MAX_DP) dp--;	// 0�ł̏��Z���
					yc = lgp->y + fp->track[LOGO_PY]*16;
					yc = (ptr->y*LOGO_MAX_DP - yc*dp +(LOGO_MAX_DP-dp)/2) /(LOGO_MAX_DP - dp);	// �t�Z
					ptr->y = Clamp(yc,-128,4096+128);
				}

				// �F��(��)
				dp = (lgp->dp_cb * fp->track[LOGO_CBDP] * fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					if(dp==LOGO_MAX_DP) dp--;	// 0�ł̏��Z���
					yc = lgp->cb + fp->track[LOGO_CB]*16;
					yc = (ptr->cb*LOGO_MAX_DP - yc*dp +(LOGO_MAX_DP-dp)/2) /(LOGO_MAX_DP - dp);
					ptr->cb = Clamp(yc,-2048-128,2048+128);
				}

				// �F��(��)
				dp = (lgp->dp_cr * fp->track[LOGO_CRDP] * fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					if(dp==LOGO_MAX_DP) dp--;	// 0�ł̏��Z���
					yc = lgp->cr + fp->track[LOGO_CR]*16;
					yc = (ptr->cr*LOGO_MAX_DP - yc*dp +(LOGO_MAX_DP-dp)/2) /(LOGO_MAX_DP - dp);
					ptr->cr = Clamp(yc,-2048-128,2048+128);
				}

			}	// if��ʓ�

			++ptr;	// �ׂ��
			++lgp;
		}
		// ���̃��C����
		ptr += fpip->max_w - j;
	}

	return TRUE;
}

/*--------------------------------------------------------------------
* 	func_proc_add_logo()	���S�t�����[�h
*-------------------------------------------------------------------*/
BOOL func_proc_add_logo(FILTER *fp,FILTER_PROC_INFO *fpip,LOGO_HEADER *lgh,int fade)
{
	int   i,j;
	int   yc;
	PIXEL_YC    *ptr;
	LOGO_PIXEL  *lgp;
	int  dp;


	// LOGO_PIXEL�f�[�^�ւ̃|�C���^
	(void *)lgp = lgh + 1;

	// ����̈ʒu�ֈړ�
	ptr = fpip->ycp_edit;
	ptr += lgh->x + lgh->y * fpip->max_w;

	for(i=0;i < lgh->h;++i){
		for(j=0;j < lgh->w;++j){

			if(ptr >= fpip->ycp_edit &&	// ��ʓ��̎��̂ݏ���
			   ptr < fpip->ycp_edit + fpip->max_w*fpip->h)
			{
				// �P�x
				dp = (lgp->dp_y * fp->track[LOGO_YDP] *fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					yc = lgp->y    + fp->track[LOGO_PY]*16;
					yc = (ptr->y*(LOGO_MAX_DP-dp) + yc*dp +(LOGO_MAX_DP/2)) /LOGO_MAX_DP;	// ���S�t��
					ptr->y = Clamp(yc,-128,4096+128);
				}


				// �F��(��)
				dp = (lgp->dp_cb * fp->track[LOGO_CBDP] *fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					yc = lgp->cb   + fp->track[LOGO_CB]*16;
					yc = (ptr->cb*(LOGO_MAX_DP-dp) + yc*dp +(LOGO_MAX_DP/2)) /LOGO_MAX_DP;
					ptr->cb = Clamp(yc,-2048-128,2048+128);
				}

				// �F��(��)
				dp = (lgp->dp_cr * fp->track[LOGO_CRDP] * fade +64)/128 /LOGO_FADE_MAX;
				if(dp){
					yc = lgp->cr   + fp->track[LOGO_CR]*16;
					yc = (ptr->cr*(LOGO_MAX_DP-dp) + yc*dp +(LOGO_MAX_DP/2)) /LOGO_MAX_DP;
					ptr->cr = Clamp(yc,-2048-128,2048+128);
				}

			}	// if��ʓ�

			++ptr;	// �ׂ��
			++lgp;
		}
		// ���̃��C����
		ptr += fpip->max_w - j;
	}

	return TRUE;
}

/*--------------------------------------------------------------------
* 	find_logo()		���S�����烍�S�f�[�^������
*-------------------------------------------------------------------*/
static int find_logo(const char *logo_name)
{
	unsigned int i;

	for(i=0;i<logodata_n;++i){
		if(lstrcmp((char *)logodata[i],logo_name)==0)
			return i;
	}

	return -1;
}

/*--------------------------------------------------------------------
* 	calc_fade()		�t�F�[�h�s�����x�v�Z
*-------------------------------------------------------------------*/
static int calc_fade(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	int fade;
	int s,e;

	if(fp->check[LOGO_BASEPROFILE]){
		FRAME_STATUS fs;
		int profile;
		int i;

		if(!fp->exfunc->get_frame_status(fpip->editp,fpip->frame,&fs))
			return LOGO_FADE_MAX;
		profile = fs.config;

		for(i=fpip->frame;i;--i){
			if(!fp->exfunc->get_frame_status(fpip->editp,i-1,&fs))
				return LOGO_FADE_MAX;
			if(fs.config != profile)
				break;
		}
		s = i;

		for(i=fpip->frame;i<fpip->frame_n-1;++i){
			if(!fp->exfunc->get_frame_status(fpip->editp,i+1,&fs))
				return LOGO_FADE_MAX;
			if(fs.config != profile)
				break;
		}
		e = i;
	}
	else{
		// �I��͈͎擾
		if(!fp->exfunc->get_select_frame(fpip->editp,&s,&e))
			return LOGO_FADE_MAX;
	}

	// �t�F�[�h�s�����x�v�Z
	if(fpip->frame < s+fp->track[LOGO_STRT]+fp->track[LOGO_FIN]){
		if(fpip->frame < s+fp->track[LOGO_STRT])
			return 0;	// �t�F�[�h�C���O
		else	// �t�F�[�h�C��
			fade = ((fpip->frame-s-fp->track[LOGO_STRT])*2 +1)*LOGO_FADE_MAX / (fp->track[LOGO_FIN]*2);
	}
	else if(fpip->frame > e-fp->track[LOGO_FOUT]-fp->track[LOGO_END]){
		if(fpip->frame > e-fp->track[LOGO_END])
			return 0;	// �t�F�[�h�A�E�g��
		else	// �t�F�[�h�A�E�g
			fade = ((e-fpip->frame-fp->track[LOGO_END])*2+1)*LOGO_FADE_MAX / (fp->track[LOGO_FOUT]*2);
	}
	else
		fade = LOGO_FADE_MAX;	// �ʏ�

	return fade;
}

/*====================================================================
*	�ݒ�E�B���h�E�v���V�[�W��
*===================================================================*/
BOOL func_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void *editp, FILTER *fp )
{
	static int mode = 1;	// 0:addlogo; 1:delogo

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

		case WM_FILTER_CHANGE_PARAM:
			if(fp->check[!mode]){	// ���[�h���ύX���ꂽ
				fp->check[mode] = 0;
				fp->exfunc->filter_window_update(fp);
				mode = !mode;
				return TRUE;
			}
			else if(!fp->check[mode]){
				fp->check[mode] = 1;
				fp->exfunc->filter_window_update(fp);
				return TRUE;
			}
			break;

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
					break;

				case ID_BUTTON_SYNTH:	// AviSynth�{�^��
					return on_avisynth_button(fp,editp);
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
	LOGO_FILE_HEADER lfh;

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
	ZeroMemory(&lfh,sizeof(lfh));
	strcpy(lfh.str,LOGO_FILE_HEADER_STR);
	dw = 0;
	WriteFile(hFile,&lfh,sizeof(LOGO_FILE_HEADER),&dw,NULL);
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

		lfh.logonum.l = SWAP_ENDIAN(n);
		SetFilePointer(hFile,0, 0, FILE_BEGIN);	// �擪��
		dw = 0;
		WriteFile(hFile,&lfh,sizeof(lfh),&dw,NULL);
		if(dw!=sizeof(lfh))
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

	// AviSynth�{�^��
	dialog.bt_synth = CreateWindow("BUTTON", "AviSynth", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_VCENTER,
									240,ITEM_Y-25, 63,20, hwnd, (HMENU)ID_BUTTON_SYNTH, hinst, NULL);
	SendMessage(dialog.bt_synth, WM_SETFONT, (WPARAM)dialog.font, 0);
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
	adjdata->x = data->x +(int)(fp->track[LOGO_X]-LOGO_XY_MIN)/4 + LOGO_XY_MIN/4;
	adjdata->y = data->y +(int)(fp->track[LOGO_Y]-LOGO_XY_MIN)/4 + LOGO_XY_MIN/4;

	adjdata->w = w = data->w + 1;	// 1/4�P�ʒ������邽��
	adjdata->h = h = data->h + 1;	// ���A�������P���₷

	// LOGO_PIXEL�̐擪
	(void *)df = (void *)(data +1);
	(void *)ex = (void *)(adjdata +1);

	adjx = (fp->track[LOGO_X]-LOGO_XY_MIN) % 4;	// �ʒu�[��
	adjy = (fp->track[LOGO_Y]-LOGO_XY_MIN) % 4;

	//----------------------------------------------------- ��ԏ�̗�
	ex[0].dp_y  = df[0].dp_y *(4-adjx)*(4-adjy)/16;	// ���[
	ex[0].dp_cb = df[0].dp_cb*(4-adjx)*(4-adjy)/16;
	ex[0].dp_cr = df[0].dp_cr*(4-adjx)*(4-adjy)/16;
	ex[0].y  = df[0].y;
	ex[0].cb = df[0].cb;
	ex[0].cr = df[0].cr;
	for(i=1;i<w-1;++i){									//��
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
	for(j=1;j<h-1;++j){
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
		for(i=1;i<w-1;++i){	//------------------ ��
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
	for(i=1;i<w-1;++i){		// ��
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
static void change_param(FILTER* fp)
{
	LRESULT ret;

	// �I��ԍ��擾
	ret = SendMessage(dialog.cb_logo,CB_GETCURSEL,0,0);
	ret = SendMessage(dialog.cb_logo,CB_GETITEMDATA,ret,0);

	if(ret!=CB_ERR)
		memcpy(ex_data,(void *)ret,LOGO_MAX_NAME);	// ���S�����R�s�[

	// �J�n��t�F�[�h�C����A�E�g��I���̏����l������Ƃ��̓p�����^�ɔ��f
	ret = find_logo((char *)ret);
	if(ret<0) return;

	if(logodata[ret]->fi || logodata[ret]->fo || logodata[ret]->st || logodata[ret]->ed){
		fp->track[LOGO_STRT] = logodata[ret]->st;
		fp->track[LOGO_FIN]  = logodata[ret]->fi;
		fp->track[LOGO_FOUT] = logodata[ret]->fo;
		fp->track[LOGO_END]  = logodata[ret]->ed;
		fp->exfunc->filter_window_update(fp);
	}
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
	LOGO_FILE_HEADER lfh;
	LOGO_HEADER lgh;
	DWORD readed = 0;
	ULONG ptr;
	void* data;
	int i;
	int same;
	int logonum;

	// �t�@�C���I�[�v��
	hFile = CreateFile(fname,GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile==INVALID_HANDLE_VALUE){
		MessageBox(fp->hwnd,"���S�f�[�^�t�@�C����������܂���",filter_name,MB_OK|MB_ICONERROR);
		return;
	}
	if(GetFileSize(hFile, NULL)<sizeof(LOGO_FILE_HEADER)){	// �T�C�Y�m�F
		CloseHandle(hFile);
		MessageBox(fp->hwnd,"���S�f�[�^�t�@�C�����s���ł�",filter_name,MB_OK|MB_ICONERROR);
		return;
	}

//	SetFilePointer(hFile,31, 0, FILE_BEGIN);	// �擪����31byte��
	ReadFile(hFile,&lfh,sizeof(LOGO_FILE_HEADER),&readed,NULL);	// �t�@�C���w�b�_�擾

	logodata_n = 0;	// �������݃f�[�^�J�E���^
	logodata = NULL;
	logonum = SWAP_ENDIAN(lfh.logonum.l);

	for(i=0;i<logonum;i++){

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
	static char message[256];
	void *ptr;
	UINT same;
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

	lstrcpy(fp->ex_data_ptr,ptr);	// �g���̈�Ƀ��S�����R�s�[
}


/*--------------------------------------------------------------------
*	on_avisynth_button()	AviSynth�{�^������
*-------------------------------------------------------------------*/
static BOOL on_avisynth_button(FILTER* fp,void *editp)
{
	static char str[STRDLG_MAXSTR];
	int  s,e;

	// �X�N���v�g����
	wsprintf(str,"%sLOGO(logofile=\"%s\",\r\n"
	             "\\           logoname=\"%s\"",
				(fp->check[0]? "Add":"Erase"),logodata_file,fp->ex_data_ptr);

	if(fp->track[LOGO_X] || fp->track[LOGO_Y])
		wsprintf(str,"%s,\r\n\\           pos_x=%d, pos_y=%d",
					str,fp->track[LOGO_X],fp->track[LOGO_Y]);

	if(fp->track[LOGO_YDP]!=128 || fp->track[LOGO_PY] || fp->track[LOGO_CB] || fp->track[LOGO_CR])
		wsprintf(str,"%s,\r\n\\           depth=%d, yc_y=%d, yc_u=%d, yc_v=%d",
					str,fp->track[LOGO_YDP],fp->track[LOGO_PY],fp->track[LOGO_CB],fp->track[LOGO_CR]);


	if(fp->exfunc->get_frame_n(editp)){	// �摜���ǂݍ��܂�Ă���Ƃ�
		fp->exfunc->get_select_frame(editp,&s,&e);	// �I��͈͎擾
		wsprintf(str,"%s,\r\n\\           start=%d",str, s+fp->track[LOGO_STRT]);

		if(fp->track[LOGO_FIN] || fp->track[LOGO_FOUT])
			wsprintf(str,"%s, fadein=%d, fadeout=%d",str,fp->track[LOGO_FIN],fp->track[LOGO_FOUT]);

		wsprintf(str,"%s, end=%d",str,e-fp->track[LOGO_END]);
	}
	else{
		if(fp->track[LOGO_FIN] || fp->track[LOGO_FOUT])
			wsprintf(str,"%s,\r\n\\           fadein=%d, fadeout=%d",str,fp->track[LOGO_FIN],fp->track[LOGO_FOUT]);
	}

	wsprintf(str,"%s)\r\n",str);

	
	EnableWindow(dialog.bt_synth,FALSE);	// synth�{�^��������

	// �_�C�A���O�Ăяo��
	DialogBoxParam(fp->dll_hinst,"STR_DLG",GetWindow(fp->hwnd,GW_OWNER),StrDlgProc,(LPARAM)str);

	EnableWindow(dialog.bt_synth,TRUE);	// synth�{�^������������

	return TRUE;
}



/*********************************************************************
*	DLLMain
*********************************************************************/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
#define TRACK_N track_N
#define CHECK_N check_N
#define FILTER_NAME_MAX  32
#define FILTER_TRACK_MAX 16
#define FILTER_CHECK_MAX 32

  //FILTER filter = ::filter;
  static char *strings[1+TRACK_N+CHECK_N];
  char key[16];
  char ini_name[MAX_PATH];
  int i;

  switch(fdwReason){
    case DLL_PROCESS_ATTACH:	// �J�n��
      // ini�t�@�C�������擾
      GetModuleFileName(hinstDLL,ini_name,MAX_PATH-4);
      strcat(ini_name,".ini");

      // �t�B���^��
      strings[0] = malloc(FILTER_NAME_MAX);
      if(strings[0]==NULL) break;
      GetPrivateProfileString("string","name",filter.name,strings[0],FILTER_NAME_MAX,ini_name);
      filter.name = strings[0];

      // �g���b�N��
      for(i=0;i<TRACK_N;i++){
        strings[i+1] = malloc(FILTER_TRACK_MAX);
        if(strings[i+1]==NULL) break;
        wsprintf(key,"track%d",i);
        GetPrivateProfileString("string",key,filter.track_name[i],strings[i+1],FILTER_TRACK_MAX,ini_name);
        filter.track_name[i] = strings[i+1];
      }

      // �`�F�b�N��
      for(i=0;i<CHECK_N;i++){
        strings[i+TRACK_N+1] = malloc(FILTER_CHECK_MAX);
        if(strings[i+TRACK_N+1]==NULL) break;
        wsprintf(key,"check%d",i);
        GetPrivateProfileString("string",key,filter.check_name[i],strings[i+TRACK_N+1],FILTER_CHECK_MAX,ini_name);
        filter.check_name[i] = strings[i+TRACK_N+1];
      }
      break;

    case DLL_PROCESS_DETACH:	// �I����
      // strings��j��
      for(i=0;i<1+TRACK_N+CHECK_N && strings[i];i++)
        free(strings[i]);
      break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
      break;
  }
  return TRUE;
}

//*/
