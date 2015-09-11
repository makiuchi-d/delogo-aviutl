//----------------------------------------------------------------------------------
//	�t�B���^�v���O�C�� �w�b�_�[�t�@�C�� for AviUtl version 0.99 �ȍ~
//	By �j�d�m����
//----------------------------------------------------------------------------------

#ifndef ___FILTER_H
#define ___FILTER_H

//	YC�\����
typedef	struct {
	short	y;					//	��f(�P�x    )�f�[�^ (     0 �` 4096 )
	short	cb;					//	��f(�F��(��))�f�[�^ ( -2048 �` 2048 )
	short	cr;					//	��f(�F��(��))�f�[�^ ( -2048 �` 2048 )
								//	��f�f�[�^�͔͈͊O�ɏo�Ă��邱�Ƃ�����܂�
								//	�܂��͈͓��Ɏ��߂Ȃ��Ă����܂��܂���
} PIXEL_YC;

//	PIXEL�\����
typedef	struct {
	unsigned char	b,g,r;		//	��f(RGB)�f�[�^ (0�`255)
} PIXEL;

//	�t�B���^PROC�p�\����
typedef struct {
	int			flag;			//	�t�B���^�̃t���O
								//	FILTER_PROC_INFO_FLAG_INVERT_FIELD_ORDER	: �t�B�[���h�I�[�_�[��W���Ƌt�Ɉ��� ( �W���̓{�g��->�g�b�v�ɂȂ��Ă��܂� )
								//	FILTER_PROC_INFO_FLAG_INVERT_INTERLACE		: �������@�𔽓]���� ( �C���^�[���[�X�����t�B���^�̂� )
	PIXEL_YC	*ycp_edit;		//	�摜�f�[�^�ւ̃|�C���^ ( ycp_edit��ycp_temp�͓���ւ���܂� )
	PIXEL_YC	*ycp_temp;		//	�e���|�����̈�ւ̃|�C���^
	int			w,h;			//	���݂̉摜�̃T�C�Y ( �摜�T�C�Y�͕ύX�o���܂� )
	int			max_w,max_h;	//	�摜�̈�̃T�C�Y
	int			frame;			//	���݂̃t���[���ԍ�( �ԍ���0���� )
	int			frame_n;		//	���t���[����
	int			org_w,org_h;	//	���̉摜�̃T�C�Y
	short		*audiop;		//	�I�[�f�B�I�f�[�^�ւ̃|�C���^ ( �I�[�f�B�I�t�B���^�̎��̂� )
								//	�I�[�f�B�I�`����PCM16bit�ł� ( 1�T���v���� mono = 2byte , stereo = 4byte )
	int			audio_n;		//	�I�[�f�B�I�T���v���̑���
	int			audio_ch;		//	�I�[�f�B�I�`�����l����
	PIXEL		*pixelp;		//	���݂͎g�p����Ă��܂���
	void		*editp;			//	�G�f�B�b�g�n���h��
	int			yc_size;		//	�摜�̈�̉�f�̃o�C�g�T�C�Y
	int			line_size;		//	�摜�̈�̕��̃o�C�g�T�C�Y
	int			reserve[8];		//	�g���p�ɗ\�񂳂�Ă܂�
} FILTER_PROC_INFO;
#define	FILTER_PROC_INFO_FLAG_INVERT_FIELD_ORDER	0x00010000
#define	FILTER_PROC_INFO_FLAG_INVERT_INTERLACE		0x00020000
//	���C���^�[���[�X�����t�B���^����ycp_edit�ɏ����摜�f�[�^�������Ă��܂���B
//	���C���^�[���[�X�����t�B���^����ycp_edit,ycp_temp,w,h��ύX�ł��܂���B

//	�t���[���X�e�[�^�X�\����
typedef struct {
	int		video;			//	���ۂ̉f���f�[�^�ԍ�
	int		audio;			//	���ۂ̉����f�[�^�ԍ�
	int		inter;			//	�t���[���̃C���^�[���[�X
							//	FRAME_STATUS_INTER_NORMAL	: �W��
							//	FRAME_STATUS_INTER_REVERSE	: ���]
							//	FRAME_STATUS_INTER_ODD		: �
							//	FRAME_STATUS_INTER_EVEN		: ����
							//	FRAME_STATUS_INTER_MIX		: ��d��
							//	FRAME_STATUS_INTER_AUTO		: ����
	int		index24fps;		//	���݂͎g�p����Ă��܂���
	int		config;			//	�t���[���̐ݒ���̔ԍ�
	int		vcm;			//	�t���[���̈��k�ݒ�̔ԍ�
	int		edit_flag;		//	�ҏW�t���O
							//	EDIT_FRAME_EDIT_FLAG_KEYFRAME	: �L�[�t���[��
							//	EDIT_FRAME_EDIT_FLAG_MARKFRAME	: �}�[�N�t���[��
							//	EDIT_FRAME_EDIT_FLAG_DELFRAME	: �D��Ԉ����t���[��
							//	EDIT_FRAME_EDIT_FLAG_NULLFRAME	: �R�s�[�t���[��
	int		reserve[9];		//	�g���p�ɗ\�񂳂�Ă܂�
} FRAME_STATUS;
#define	FRAME_STATUS_INTER_NORMAL		0
#define	FRAME_STATUS_INTER_REVERSE		1
#define	FRAME_STATUS_INTER_ODD			2
#define	FRAME_STATUS_INTER_EVEN			3
#define	FRAME_STATUS_INTER_MIX			4
#define	FRAME_STATUS_INTER_AUTO			5
#define	EDIT_FRAME_EDIT_FLAG_KEYFRAME		1
#define	EDIT_FRAME_EDIT_FLAG_MARKFRAME		2
#define	EDIT_FRAME_EDIT_FLAG_DELFRAME		4
#define	EDIT_FRAME_EDIT_FLAG_NULLFRAME		8

//	�t�@�C���C���t�H���[�V�����\����
typedef struct {
	int		flag;					//	�t�@�C���̃t���O
									//	FILE_INFO_FLAG_VIDEO	: �f�������݂���
									//	FILE_INFO_FLAG_AUDIO	: ���������݂���
	LPSTR	name;					//	�ҏW�t�@�C����
	int		w,h;					//	���̃T�C�Y
	int		video_rate,video_scale;	//	�t���[�����[�g
	int		audio_rate;				//	�����T���v�����O���[�g
	int		audio_ch;				//	�����`�����l����
	int		reserve[8];				//	�g���p�ɗ\�񂳂�Ă܂�
} FILE_INFO;
#define FILE_INFO_FLAG_VIDEO	1
#define FILE_INFO_FLAG_AUDIO	2

//	�V�X�e���C���t�H���[�V�����\����
typedef struct {
	int		flag;					//	�V�X�e���t���O
									//	SYS_INFO_FLAG_EDIT		: �ҏW��
									//	SYS_INFO_FLAG_VFAPI		: VFAPI���쎞
									//	SYS_INFO_FLAG_USE_SSE	: SSE�g�p
									//	SYS_INFO_FLAG_USE_SSE2	: SSE2�g�p
	LPSTR	info;					//	�o�[�W�������
	int		filter_n;				//	�o�^����Ă�t�B���^�̐�
	int		min_w,min_h;			//	�ҏW�o����ŏ��摜�T�C�Y
	int		max_w,max_h;			//	�ҏW�o����ő�摜�T�C�Y
	int		max_frame;				//	�ҏW�o����ő�t���[����
	LPSTR	edit_name;				//	�ҏW�t�@�C���� (�t�@�C���������܂��Ă��Ȃ����͉��������Ă��܂���)
	LPSTR	project_name;			//	�v���W�F�N�g�t�@�C���� (�t�@�C���������܂��Ă��Ȃ����͉��������Ă��܂���)
	LPSTR	output_name;			//	�o�̓t�@�C���� (�t�@�C���������܂��Ă��Ȃ����͉��������Ă��܂���)
	int		vram_w,vram_h;			//	�ҏW�p�摜�̈�̃T�C�Y
	int		vram_yc_size;			//	�ҏW�p�摜�̈�̉�f�̃o�C�g�� ( PIXEL_YC = 6 , PIXEL_YUY2 = 2 )
	int		vram_line_size;			//	�ҏW�p�摜�̈�̕��̃o�C�g��
	int		reserve[4];				//	�g���p�ɗ\�񂳂�Ă܂�
} SYS_INFO;
#define SYS_INFO_FLAG_EDIT		1
#define SYS_INFO_FLAG_VFAPI		2
#define SYS_INFO_FLAG_USE_SSE	4
#define SYS_INFO_FLAG_USE_SSE2	8

//	�O���֐��\����
typedef struct {
	void		*(*get_ycp_ofs)( void *editp,int n,int ofs );
								//	���o���邾��get_ycp_source_cache()�̕����g�p����悤�ɂ��Ă�������
								//	�w�肵���t���[����AVI�t�@�C����ł̃I�t�Z�b�g���ړ�����
								//	�t���[���̉摜�f�[�^�̃|�C���^���擾���܂�
								//	�f�[�^�̓t�B���^�O�̂��̂ł�
								//	editp 	: �G�f�B�b�g�n���h��
								//	n	 	: �t���[���ԍ�
								//	ofs	 	: �t���[������̃I�t�Z�b�g
								//  �߂�l	: �摜�f�[�^�ւ̃|�C���^ (NULL�Ȃ玸�s)
								//			  �摜�f�[�^�|�C���^�̓��e�͎��ɊO���֐����g�������C���ɏ�����߂��܂ŗL��
	void		*(*get_ycp)( void *editp,int n );
								//	���o���邾��get_ycp_source_cache()�̕����g�p����悤�ɂ��Ă�������
								//	�w�肵���t���[���̉摜�f�[�^�̃|�C���^���擾���܂�
								//	�f�[�^�̓t�B���^�O�̂��̂ł�
								//	editp 	: �G�f�B�b�g�n���h��
								//	n	 	: �t���[���ԍ�
								//  �߂�l	: �摜�f�[�^�ւ̃|�C���^ (NULL�Ȃ玸�s)
								//			  �摜�f�[�^�|�C���^�̓��e�͎��ɊO���֐����g�������C���ɏ�����߂��܂ŗL��
	void		*(*get_pixelp)( void *editp,int n );
								//	�w�肵���t���[����DIB�`��(RGB24bit)�̉摜�f�[�^�̃|�C���^���擾���܂�
								//	�f�[�^�̓t�B���^�O�̂��̂ł�
								//	editp 	: �G�f�B�b�g�n���h��
								//	n		: �t���[���ԍ�
								//  �߂�l	: DIB�`���f�[�^�ւ̃|�C���^ (NULL�Ȃ玸�s)
								//			  �摜�f�[�^�|�C���^�̓��e�͎��ɊO���֐����g�������C���ɏ�����߂��܂ŗL��
	int			(*get_audio)( void *editp,int n,void *buf );
								//	�w�肵���t���[���̃I�[�f�B�I�f�[�^��ǂݍ��݂܂�
								//	�f�[�^�̓t�B���^�O�̂��̂ł�
								//	editp 	: �G�f�B�b�g�n���h��
								//	n		: �t���[���ԍ�
								//	buf 	: �i�[����o�b�t�@ (NULL�Ȃ�T���v�����̎擾�̂�)
								//  �߂�l	: �ǂݍ��񂾃T���v����
	BOOL		(*is_editing)( void *editp );
								//	���ݕҏW�������ׂ܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  �߂�l	: TRUE�Ȃ�ҏW��
	BOOL		(*is_saving)( void *editp );
								//	���ݕۑ��������ׂ܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  �߂�l	: TRUE�Ȃ�ۑ���
	int			(*get_frame)( void *editp );
								//	���݂̕\���t���[�����擾���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  �߂�l	: ���݂̃t���[���ԍ�
	int			(*get_frame_n)( void *editp );
								//	���t���[�������擾���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  �߂�l	: ���݂̑��t���[����
	BOOL		(*get_frame_size)( void *editp,int *w,int *h );
								//	�t�B���^�O�̃t���[���̃T�C�Y���擾���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//	w,h 	: �摜�T�C�Y�̊i�[�|�C���^
								//  �߂�l	: TRUE�Ȃ琬��
	int			(*set_frame)( void *editp,int n );
								//	���݂̕\���t���[����ύX���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  n		: �t���[���ԍ�
								//  �߂�l	: �ݒ肳�ꂽ�t���[���ԍ�
	int			(*set_frame_n)( void *editp,int n );
								//	���t���[������ύX���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  n		: �t���[����
								//  �߂�l	: �ݒ肳�ꂽ���t���[����
	BOOL		(*copy_frame)( void *editp,int d,int s );
								//	�t���[���𑼂̃t���[���ɃR�s�[���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//	d	 	: �R�s�[��t���[���ԍ�
								//	s	 	: �R�s�[���t���[���ԍ�
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*copy_video)( void *editp,int d,int s );
								//	�t���[���̉f�������𑼂̃t���[���ɃR�s�[���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//	d	 	: �R�s�[��t���[���ԍ�
								//	s	 	: �R�s�[���t���[���ԍ�
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*copy_audio)( void *editp,int d,int s );
								//	�t���[���̉��������𑼂̃t���[���ɃR�s�[���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//	d	 	: �R�s�[��t���[���ԍ�
								//	s	 	: �R�s�[���t���[���ԍ�
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*copy_clip)( HWND hwnd,void *pixelp,int w,int h );
								//	�N���b�v�{�[�h��DIB�`��(RGB24bit)�̉摜���R�s�[���܂�
								//	hwnd 	: �E�B���h�E�n���h��
								//	pixelp	: DIB�`���f�[�^�ւ̃|�C���^
								//	w,h 	: �摜�T�C�Y
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*paste_clip)( HWND hwnd,void *editp,int n );
								//	�N���b�v�{�[�h����摜�𒣂���܂�
								//	hwnd 	: �E�B���h�E�n���h��
								//	editp 	: �G�f�B�b�g�n���h��
								//  n		: �t���[���ԍ�
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*get_frame_status)( void *editp,int n,FRAME_STATUS *fsp );
								//	�t���[���̃X�e�[�^�X���擾���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  n		: �t���[���ԍ�
								//  fps		: �t���[���X�e�[�^�X�ւ̃|�C���^
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*set_frame_status)( void *editp,int n,FRAME_STATUS *fsp );
								//	�t���[���̃X�e�[�^�X��ύX���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  n		: �t���[���ԍ�
								//  fps		: �t���[���X�e�[�^�X�ւ̃|�C���^
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*is_saveframe)( void *editp,int n );
								//	���ۂɕۑ������t���[�������ׂ܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  n		: �t���[���ԍ�
								//  �߂�l	: TRUE�Ȃ�ۑ�����܂�
	BOOL		(*is_keyframe)( void *editp,int n );
								//	�L�[�t���[�����ǂ������ׂ܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  n		: �t���[���ԍ�
								//  �߂�l	: TRUE�Ȃ�L�[�t���[��
	BOOL		(*is_recompress)( void *editp,int n );
								//	�Ĉ��k���K�v�����ׂ܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  n		: �t���[���ԍ�
								//  �߂�l	: TRUE�Ȃ�Ĉ��k���K�v
	BOOL		(*filter_window_update)( void *fp );
								//	�ݒ�E�B���h�E�̃g���b�N�o�[�ƃ`�F�b�N�{�b�N�X���ĕ`�悵�܂�
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*is_filter_window_disp)( void *fp );
								//	�ݒ�E�B���h�E���\������Ă��邩���ׂ܂�
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//  �߂�l	: TRUE�Ȃ�\������Ă���
	BOOL		(*get_file_info)( void *editp,FILE_INFO *fip );
								//	�ҏW�t�@�C���̏����擾���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  fip		: �t�@�C���C���t�H���[�V�����\���̂ւ̃|�C���^
								//  �߂�l	: TRUE�Ȃ琬��
	LPSTR		(*get_config_name)( void *editp,int n );
								//	�ݒ���̖��O���擾���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  n		: �ݒ���̔ԍ�
								//  �߂�l	: �ݒ���̖��O�ւ̃|�C���^
	BOOL		(*is_filter_active)( void *fp );
								//	�t�B���^���L���ɂȂ��Ă��邩���ׂ܂�
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//  �߂�l	: TRUE�Ȃ�t�B���^�L��
	BOOL		(*get_pixel_filtered)( void *editp,int n,void *pixelp,int *w,int *h );
								//	�w�肵���t���[����DIB�`��(RGB24bit)�̉摜�f�[�^��ǂݍ��݂܂�
								//	�f�[�^�̓t�B���^��̂��̂ł�
								//	editp 	: �G�f�B�b�g�n���h��
								//	n		: �t���[���ԍ�
								//  pixelp	: DIB�`���f�[�^���i�[����|�C���^ (NULL�Ȃ�摜�T�C�Y������Ԃ��܂�)
								//	w,h		: �摜�̃T�C�Y (NULL�Ȃ�DIB�`���f�[�^������Ԃ��܂�)
								//  �߂�l	: TRUE�Ȃ琬��
	int			(*get_audio_filtered)( void *editp,int n,void *buf );
								//	�w�肵���t���[���̃I�[�f�B�I�f�[�^��ǂݍ��݂܂�
								//	�f�[�^�̓t�B���^��̂��̂ł�
								//	editp* 	: �G�f�B�b�g�n���h��
								//	n		: �t���[���ԍ�
								//	buf 	: �i�[����o�b�t�@ (NULL�Ȃ�T���v�����̎擾�̂�)
								//  �߂�l	: �ǂݍ��񂾃T���v����
	BOOL		(*get_select_frame)( void *editp,int *s,int *e );
								//	�I���J�n�I���t���[�����擾���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//	s		: �I���J�n�t���[��
								//	e		: �I���I���t���[��
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*set_select_frame)( void *editp,int s,int e );
								//	�I���J�n�I���t���[����ݒ肵�܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//	s		: �I���J�n�t���[��
								//	e		: �I���I���t���[��
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*rgb2yc)( PIXEL_YC *ycp,PIXEL *pixelp,int w );
								//	PIXEL����PIXEL_YC�ɕϊ����܂�
								//	ycp		: PIXEL_YC�\���̂ւ̃|�C���^
								//	pixelp 	: PIXEL�\���̂ւ̃|�C���^
								//	w		: �\���̂̐�
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*yc2rgb)( PIXEL *pixelp,PIXEL_YC *ycp,int w );
								//	PIXEL_YC����PIXEL�ɕϊ����܂�
								//	pixelp 	: PIXEL�\���̂ւ̃|�C���^
								//	ycp		: PIXEL_YC�\���̂ւ̃|�C���^
								//	w		: �\���̂̐�
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*dlg_get_load_name)( LPSTR name,LPSTR filter,LPSTR def );
								//	�t�@�C���_�C�A���O���g���ēǂݍ��ރt�@�C�������擾���܂�
								//	name	: �t�@�C�������i�[����|�C���^
								//	filter	: �t�@�C���t�B���^
								//  def		: �f�t�H���g�̃t�@�C����
								//  �߂�l	: TRUE�Ȃ琬�� FALSE�Ȃ�L�����Z��
	BOOL		(*dlg_get_save_name)( LPSTR name,LPSTR filter,LPSTR def );
								//	�t�@�C���_�C�A���O���g���ď������ރt�@�C�������擾���܂�
								//	name	: �t�@�C�������i�[����|�C���^
								//	filter	: �t�@�C���t�B���^
								//  def		: �f�t�H���g�̃t�@�C����
								//  �߂�l	: TRUE�Ȃ琬�� FALSE�Ȃ�L�����Z��
	int			(*ini_load_int)( void *fp,LPSTR key,int n );
								//	INI�t�@�C�����琔�l��ǂݍ���
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//	key		: �A�N�Z�X�p�̃L�[�̖��O
								//  n		: �f�t�H���g�̐��l
								//  �߂�l	: �ǂݍ��񂾐��l
	int			(*ini_save_int)( void *fp,LPSTR key,int n );
								//	INI�t�@�C���ɐ��l����������
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//	key		: �A�N�Z�X�p�̃L�[�̖��O
								//  n		: �������ސ��l
								//  �߂�l	: �������񂾐��l
	BOOL		(*ini_load_str)( void *fp,LPSTR key,LPSTR str,LPSTR def );
								//	INI�t�@�C�����當�����ǂݍ���
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//	key		: �A�N�Z�X�p�̃L�[�̖��O
								//  str		: �������ǂݍ��ރo�b�t�@
								//  def		: �f�t�H���g�̕�����
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*ini_save_str)( void *fp,LPSTR key,LPSTR str );
								//	INI�t�@�C���ɕ��������������
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//	key		: �A�N�Z�X�p�̃L�[�̖��O
								//  n		: �������ޕ�����
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*get_source_file_info)( void *editp,FILE_INFO *fip,int source_file_id );
								//	�w�肵���t�@�C��ID�̃t�@�C���̏����擾���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//  fip		: �t�@�C���C���t�H���[�V�����\���̂ւ̃|�C���^
								//	souce_file_id
								//			: �t�@�C��ID
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*get_source_video_number)( void *editp,int n,int *source_file_id,int *source_video_number );
								//	�w�肵���t���[���̃\�[�X�̃t�@�C��ID�ƃt���[���ԍ����擾���܂�
								//	editp 	: �G�f�B�b�g�n���h��
								//	n		: �t���[���ԍ�
								//	souce_file_id
								//			: �t�@�C��ID���i�[����|�C���^
								//	souce_video_number
								//			: �t���[���ԍ����i�[����|�C���^
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*get_sys_info)( void *editp,SYS_INFO *sip );
								//	�V�X�e���̏����擾���܂�
								//	editp 	: �G�f�B�b�g�n���h�� (NULL�Ȃ�sip�̕ҏW���̃t���O�Ƃ��ׂẴt�@�C�����������ɂȂ�܂�)
								//  sip		: �V�X�e���C���t�H���[�V�����\���̂ւ̃|�C���^
								//  �߂�l	: TRUE�Ȃ琬��
	void 		*(*get_filterp)( int filter_id );
								//	�w��̃t�B���^ID�̃t�B���^�\���̂ւ̃|�C���^���擾���܂�
								//	filter_id
								//		 	: �t�B���^ID (0�`�o�^����Ă�t�B���^�̐�-1�܂ł̒l)
								//  �߂�l	: �t�B���^�\���̂ւ̃|�C���^ (NULL�Ȃ玸�s)
	void		*(*get_ycp_filtering)( void *fp,void *editp,int n,void *reserve );
								//	���o���邾��get_ycp_filtering_cache_ex()�̕����g�p����悤�ɂ��Ă�������
								//	�w�肵���t���[���̉摜�f�[�^�̃|�C���^���擾���܂�
								//	�f�[�^�͎����̃t�B���^�̒��O�܂Ńt�B���^�������̂ł�
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//	editp 	: �G�f�B�b�g�n���h��
								//	n	 	: �t���[���ԍ�
								//	reserve	: NULL���w�肵�Ă�������
								//  �߂�l	: �摜�f�[�^�ւ̃|�C���^ (NULL�Ȃ玸�s)
								//			  �摜�f�[�^�|�C���^�̓��e�͎��ɊO���֐����g�������C���ɏ�����߂��܂ŗL��
	int			(*get_audio_filtering)( void *fp,void *editp,int n,void *buf );
								//	�w�肵���t���[���̃I�[�f�B�I�f�[�^��ǂݍ��݂܂�
								//	�f�[�^�͎����̃t�B���^�̒��O�܂Ńt�B���^�������̂ł�
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//	editp 	: �G�f�B�b�g�n���h��
								//	n		: �t���[���ԍ�
								//	buf 	: �i�[����o�b�t�@ (NULL�Ȃ�T���v�����̎擾�̂�)
								//  �߂�l	: �ǂݍ��񂾃T���v����
	BOOL		(*set_ycp_filtering_cache_size)( void *fp,int w,int h,int d,int flag );
								//	get_ycp_filtering_cache_ex()�̃L���b�V���̐ݒ�����܂�
								//	�ݒ�l���ς�������̂݃L���b�V���̈���Ċm�ۂ��܂�
								//	�L���b�V���̈�̓t�B���^���A�N�e�B�u�̎��̂݊m�ۂ���܂�
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//	w	 	: �L���b�V���̈�̕�
								//	h	 	: �L���b�V���̈�̍���
								//	d	 	: �L���b�V������t���[����
								//	flag 	: NULL���w�肵�Ă�������
								//  �߂�l	: TRUE�Ȃ琬��
	void		*(*get_ycp_filtering_cache)( void *fp,void *editp,int n );
								//	���o���邾��get_ycp_filtering_cache_ex()�̕����g�p����悤�ɂ��Ă�������
								//	�w�肵���t���[���̉摜�f�[�^�̃L���b�V���|�C���^���擾���܂�
								//	set_ycp_filtering_cache_size()�̐ݒ�ɂ��������ăL���b�V������܂�
								//	�f�[�^�͎����̃t�B���^�̒��O�܂Ńt�B���^�������̂ł�
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//	editp 	: �G�f�B�b�g�n���h��
								//	n	 	: �t���[���ԍ�
								//  �߂�l	: �摜�f�[�^�ւ̃L���b�V���|�C���^ (NULL�Ȃ玸�s)
								//			  �摜�f�[�^�|�C���^�̓��e�̓L���b�V������j�������܂ŗL��
	void		*(*get_ycp_source_cache)( void *editp,int n,int ofs );
								//	�w�肵���t���[���̉摜�f�[�^�̃|�C���^���擾���܂�
								//	�f�[�^�̓t�B���^�O�̂��̂ł�
								//	editp 	: �G�f�B�b�g�n���h��
								//	n	 	: �t���[���ԍ�
								//	ofs	 	: ����AVI��ł̃t���[���̃I�t�Z�b�g
								//  �߂�l	: �摜�f�[�^�ւ̃|�C���^ (NULL�Ȃ玸�s)
								//			  �摜�f�[�^�|�C���^�̓��e�̓L���b�V������j�������܂ŗL��
	void		*(*get_disp_pixelp)( void *editp,DWORD format );
								//	�\������Ă���t���[���̉摜�f�[�^�̃|�C���^���擾���܂�
								//	�f�[�^�̓t�B���^��̂��̂ł�
								//	�\���t�B���^�̂ݎg�p�\�ł��B
								//	editp 	: �G�f�B�b�g�n���h��
								//	format	: �摜�t�H�[�}�b�g( NULL = RGB24bit / 'Y''U''Y''2' = YUY2 )
								//  �߂�l	: �摜�f�[�^�ւ̃|�C���^ (NULL�Ȃ玸�s)
								//			  �摜�f�[�^�|�C���^�̓��e�͎��ɊO���֐����g�������C���ɏ�����߂��܂ŗL��
	BOOL		(*get_pixel_source)( void *editp,int n,void *pixelp,DWORD format );
								//	�w�肵���t���[���̉摜�f�[�^��ǂݍ��݂܂�
								//	�f�[�^�̓t�B���^�O�̂��̂ł�
								//	editp 	: �G�f�B�b�g�n���h��
								//	n	 	: �t���[���ԍ�
								//  pixelp	: DIB�`���f�[�^���i�[����|�C���^
								//	format	: �摜�t�H�[�}�b�g( NULL = RGB24bit / 'Y''U''Y''2' = YUY2 )
								//  �߂�l	: TRUE�Ȃ琬��
	BOOL		(*get_pixel_filtered_ex)( void *editp,int n,void *pixelp,int *w,int *h,DWORD format );
								//	�w�肵���t���[���̉摜�f�[�^��ǂݍ��݂܂�
								//	�f�[�^�̓t�B���^��̂��̂ł�
								//	editp 	: �G�f�B�b�g�n���h��
								//	n	 	: �t���[���ԍ�
								//  pixelp	: DIB�`���f�[�^���i�[����|�C���^ (NULL�Ȃ�摜�T�C�Y������Ԃ��܂�)
								//	w,h		: �摜�̃T�C�Y (NULL�Ȃ�DIB�`���f�[�^������Ԃ��܂�)
								//	format	: �摜�t�H�[�}�b�g( NULL = RGB24bit / 'Y''U''Y''2' = YUY2 )
								//  �߂�l	: TRUE�Ȃ琬��
	PIXEL_YC	*(*get_ycp_filtering_cache_ex)( void *fp,void *editp,int n,int *w,int *h );
								//	�w�肵���t���[���̉摜�f�[�^�̃L���b�V���|�C���^���擾���܂�
								//	set_ycp_filtering_cache_size()�̐ݒ�ɂ��������ăL���b�V������܂�
								//	�f�[�^�͎����̃t�B���^�̒��O�܂Ńt�B���^�������̂ł�
								//	fp	 	: �t�B���^�\���̂̃|�C���^
								//	editp 	: �G�f�B�b�g�n���h��
								//	n	 	: �t���[���ԍ�
								//	w,h		: �摜�̃T�C�Y (NULL�Ȃ疳������܂�)
								//  �߂�l	: �摜�f�[�^�ւ̃L���b�V���|�C���^ (NULL�Ȃ玸�s)
								//			  �摜�f�[�^�|�C���^�̓��e�̓L���b�V������j�������܂ŗL��
	int			reserve[13];
} EXFUNC;

//	�t�B���^�\����
typedef struct {
	int		flag;				//	�t�B���^�̃t���O
								//	FILTER_FLAG_ALWAYS_ACTIVE		: �t�B���^����ɃA�N�e�B�u�ɂ��܂�
								//	FILTER_FLAG_CONFIG_POPUP		: �ݒ���|�b�v�A�b�v���j���[�ɂ��܂�
								//	FILTER_FLAG_CONFIG_CHECK		: �ݒ���`�F�b�N�{�b�N�X���j���[�ɂ��܂�
								//	FILTER_FLAG_CONFIG_RADIO		: �ݒ�����W�I�{�^�����j���[�ɂ��܂�
								//	FILTER_FLAG_EX_DATA				: �g���f�[�^��ۑ��o����悤�ɂ��܂�
								//	FILTER_FLAG_PRIORITY_HIGHEST	: �t�B���^�̃v���C�I���e�B����ɍŏ�ʂɂ��܂�
								//	FILTER_FLAG_PRIORITY_LOWEST		: �t�B���^�̃v���C�I���e�B����ɍŉ��ʂɂ��܂�
								//	FILTER_FLAG_WINDOW_THICKFRAME	: �T�C�Y�ύX�\�ȃE�B���h�E�����܂�
								//	FILTER_FLAG_WINDOW_SIZE			: �ݒ�E�B���h�E�̃T�C�Y���w��o����悤�ɂ��܂�
								//	FILTER_FLAG_DISP_FILTER			: �\���t�B���^�ɂ��܂�
								//	FILTER_FLAG_REDRAW				: �ĕ`���plugin���ŏ�������悤�ɂ��܂�
								//	FILTER_FLAG_EX_INFORMATION		: �t�B���^�̊g������ݒ�ł���悤�ɂ��܂�
								//	FILTER_FLAG_INFORMATION			: FILTER_FLAG_EX_INFORMATION ���g���悤�ɂ��ĉ�����
								//	FILTER_FLAG_NO_CONFIG			: �ݒ�E�B���h�E��\�����Ȃ��悤�ɂ��܂�
								//	FILTER_FLAG_AUDIO_FILTER		: �I�[�f�B�I�t�B���^�ɂ��܂�
								//	FILTER_FLAG_RADIO_BUTTON		: �`�F�b�N�{�b�N�X�����W�I�{�^���ɂ��܂�
								//	FILTER_FLAG_WINDOW_HSCROLL		: �����X�N���[���o�[�����E�B���h�E�����܂�
								//	FILTER_FLAG_WINDOW_VSCROLL		: �����X�N���[���o�[�����E�B���h�E�����܂�
								//	FILTER_FLAG_INTERLACE_FILTER	: �C���^�[���[�X�����t�B���^�ɂ��܂�
								//	FILTER_FLAG_NO_INIT_DATA		: func_proc()�̉摜�̏����f�[�^���쐬���Ȃ��悤�ɂ��܂�
								//	FILTER_FLAG_IMPORT				: �C���|�[�g���j���[�����܂�
								//	FILTER_FLAG_EXPORT				: �G�N�X�|�[�g���j���[�����܂�
								//	FILTER_FLAG_MAIN_MESSAGE		: func_WndProc()��WM_FILTER_MAIN_???�̃��b�Z�[�W�𑗂�悤�ɂ��܂�
	int		x,y;				//	�ݒ�E�C���h�E�̃T�C�Y (FILTER_FLAG_WINDOW_SIZE�������Ă��鎞�ɗL��)
	TCHAR	*name;				//	�t�B���^�̖��O
	int		track_n;			//	�g���b�N�o�[�̐�
	TCHAR	**track_name;		//	�g���b�N�o�[�̖��O�S�ւ̃|�C���^(�g���b�N�o�[����0�Ȃ�NULL�ł悢)
	int		*track_default;		//	�g���b�N�o�[�̏����l�S�ւ̃|�C���^(�g���b�N�o�[����0�Ȃ�NULL�ł悢)
	int		*track_s,*track_e;	//	�g���b�N�o�[�̐��l�̉������ (NULL�Ȃ�S��0�`256)
	int		check_n;			//	�`�F�b�N�{�b�N�X�̐�
	TCHAR	**check_name;		//	�`�F�b�N�{�b�N�X�̖��O�S�ւ̃|�C���^(�`�F�b�N�{�b�N�X����0�Ȃ�NULL�ł悢)
	int		*check_default;		//	�`�F�b�N�{�b�N�X�̏����l�S�ւ̃|�C���^(�`�F�b�N�{�b�N�X����0�Ȃ�NULL�ł悢)
	BOOL	(*func_proc)( void *fp,FILTER_PROC_INFO *fpip );
								//	�t�B���^�����֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	BOOL	(*func_init)( void *fp );
								//	�J�n���ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	BOOL	(*func_exit)( void *fp );
								//	�I�����ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	BOOL	(*func_update)( void *fp,int status );
								//	�����̐ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
								//	FILTER_UPDATE_STATUS_ALL		: �S���ڂ��ύX���ꂽ
								//	FILTER_UPDATE_STATUS_TRACK + n	: n�Ԗڂ̃g���b�N�o�[���ύX���ꂽ
								//	FILTER_UPDATE_STATUS_CHECK + n	: n�Ԗڂ̃`�F�b�N�{�b�N�X���ύX���ꂽ
	BOOL 	(*func_WndProc)( HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam,void *editp,void *fp );
								//	�ݒ�E�B���h�E�ɃE�B���h�E���b�Z�[�W���������ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
								//	VFAPI���쎞�ɂ͌Ă΂�܂���B
								//	�ʏ�̃��b�Z�[�W�ȊO�Ɉȉ��̊g�����b�Z�[�W�������܂�
								//	WM_FILTER_UPDATE		: �e�t�B���^�ݒ��ҏW���e���ύX���ꂽ����ɑ����܂�
								//	WM_FILTER_FILE_OPEN		: �ҏW�t�@�C�����I�[�v�����ꂽ����ɑ����܂�
								//	WM_FILTER_FILE_CLOSE	: �ҏW�t�@�C�����N���[�Y����钼�O�ɑ����܂�
								//	WM_FILTER_INIT			: �J�n����ɑ����܂�
								//	WM_FILTER_EXIT			: �I�����O�ɑ����܂�
								//	WM_FILTER_SAVE_START	: �Z�[�u���J�n����钼�O�ɑ����܂�
								//	WM_FILTER_SAVE_END		: �Z�[�u���I�����ꂽ����ɑ����܂�
								//	WM_FILTER_IMPORT		: �C���|�[�g���I�����ꂽ����ɑ����܂�
								//	WM_FILTER_EXPORT		: �G�N�X�|�[�g���I�����ꂽ����ɑ����܂�
								//	WM_FILTER_CHANGE_ACTIVE	: �t�B���^�̗L��/�������ύX���ꂽ����ɑ����܂�
								//	WM_FILTER_CHANGE_WINDOW	: �t�B���^�E�B���h�E�̕\��/��\�����ύX���ꂽ����ɑ����܂�
								//	WM_FILTER_CHANGE_PARAM	: �����̃t�B���^�̐ݒ肪�ύX���ꂽ����ɑ����܂�
								//	WM_FILTER_CHANGE_EDIT	: �ҏW��/��ҏW�����ύX���ꂽ����ɑ����܂�
								//	����ȍ~�̃��b�Z�[�W��FILTER_FLAG_MAIN_INPUT_MESSAGE�ݒ莞�̂ݑ����܂�
								//	WM_FILTER_MAIN_MOUSE_DOWN	: ���C���E�B���h�E�Ń}�E�X�̃{�^���������ꂽ���ɑ����܂�
								//	WM_FILTER_MAIN_MOUSE_UP		: ���C���E�B���h�E�Ń}�E�X���ړ��������ɑ����܂�
								//	WM_FILTER_MAIN_MOUSE_MOVE	: ���C���E�B���h�E�Ń}�E�X�̃{�^���������ꂽ���ɑ����܂�
								//	WM_FILTER_MAIN_KEY_DOWN		: ���C���E�B���h�E�ŃL�[�������ꂽ���ɑ����܂�
								//	WM_FILTER_MAIN_KEY_UP		: ���C���E�B���h�E�ŃL�[�������ꂽ���ɑ����܂�
								//	WM_FILTER_MAIN_MOVESIZE		: ���C���E�B���h�E�̈ʒu��T�C�Y���ύX���ꂽ���ɑ����܂�
								//	WM_FILTER_MAIN_MOUSE_???��lparam�ɂ͕ҏW�摜��ł̍��W������܂�(�ҏW���ȊO��0�ɂȂ�܂�)
								//	WM_FILTER_MAIN_KEY_???��wparam�ɂ͉��z�L�[�R�[�h������܂�
								//	WM_FILTER_MAIN_MOVESIZE��lparam�ɂ̓��C���E�B���h�E�̃E�B���h�E�n���h��������܂�
								//	�߂�l��TRUE�ɂ���ƕҏW���e���X�V���ꂽ�Ƃ��đS�̂��ĕ`�悳��܂�
	int		*track,*check;		//	�V�X�e���Ŏg���܂��̂Ŏg�p���Ȃ��ł�������
	void	*ex_data_ptr;		//	�g���f�[�^�̈�ւ̃|�C���^ (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	int		ex_data_size;		//	�g���f�[�^�T�C�Y (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	TCHAR	*information;		//	�t�B���^���ւ̃|�C���^ (FILTER_FLAG_EX_INFORMATION�������Ă��鎞�ɗL��)
	BOOL	(*func_save_start)( void *fp,int s,int e,void *editp );
								//	�Z�[�u���J�n����钼�O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	BOOL	(*func_save_end)( void *fp,void *editp );
								//	�Z�[�u���I���������O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	EXFUNC	*exfunc;			//	�O���֐��e�[�u���ւ̃|�C���^
	HWND	hwnd;				//	�E�B���h�E�n���h��
	HINSTANCE	dll_hinst;		//	DLL�̃C���X�^���X�n���h��
	void	*ex_data_def;		//	�g���f�[�^�̏����l�f�[�^�̈�ւ̃|�C���^ (NULL�Ȃ珉��������܂���)
	BOOL	(*func_is_saveframe)( void *fp,void *editp,int saveno,int frame,int fps,int edit_flag,int inter );
								//	�C���^�[���[�X�����t�B���^�ŕۑ�����t���[�������߂鎞�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	int		reserve[6];			//	�g���p�ɗ\�񂳂�Ă܂��BNULL�ɂ��Ă��������B
} FILTER;
#define	FILTER_FLAG_ACTIVE				1
#define	FILTER_FLAG_ALWAYS_ACTIVE		4
#define	FILTER_FLAG_CONFIG_POPUP		8
#define	FILTER_FLAG_CONFIG_CHECK		16
#define	FILTER_FLAG_CONFIG_RADIO		32
#define	FILTER_FLAG_EX_DATA				1024
#define	FILTER_FLAG_PRIORITY_HIGHEST	2048
#define	FILTER_FLAG_PRIORITY_LOWEST		4096
#define	FILTER_FLAG_WINDOW_THICKFRAME	8192
#define	FILTER_FLAG_WINDOW_SIZE			16384
#define	FILTER_FLAG_DISP_FILTER			32768
#define	FILTER_FLAG_REDRAW				0x20000
#define	FILTER_FLAG_EX_INFORMATION		0x40000
#define	FILTER_FLAG_INFORMATION			0x80000
#define	FILTER_FLAG_NO_CONFIG			0x100000
#define	FILTER_FLAG_AUDIO_FILTER		0x200000
#define	FILTER_FLAG_RADIO_BUTTON		0x400000
#define	FILTER_FLAG_WINDOW_HSCROLL		0x800000
#define	FILTER_FLAG_WINDOW_VSCROLL		0x1000000
#define	FILTER_FLAG_INTERLACE_FILTER	0x4000000
#define	FILTER_FLAG_NO_INIT_DATA		0x8000000
#define	FILTER_FLAG_IMPORT				0x10000000
#define	FILTER_FLAG_EXPORT				0x20000000
#define	FILTER_FLAG_MAIN_MESSAGE		0x40000000
#define WM_FILTER_UPDATE				(WM_USER+100)
#define WM_FILTER_FILE_OPEN				(WM_USER+101)
#define WM_FILTER_FILE_CLOSE			(WM_USER+102)
#define WM_FILTER_INIT					(WM_USER+103)
#define WM_FILTER_EXIT					(WM_USER+104)
#define WM_FILTER_SAVE_START			(WM_USER+105)
#define WM_FILTER_SAVE_END				(WM_USER+106)
#define WM_FILTER_IMPORT				(WM_USER+107)
#define WM_FILTER_EXPORT				(WM_USER+108)
#define WM_FILTER_CHANGE_ACTIVE			(WM_USER+109)
#define WM_FILTER_CHANGE_WINDOW			(WM_USER+110)
#define WM_FILTER_CHANGE_PARAM			(WM_USER+111)
#define WM_FILTER_CHANGE_EDIT			(WM_USER+112)
#define	WM_FILTER_MAIN_MOUSE_DOWN		(WM_USER+120)
#define	WM_FILTER_MAIN_MOUSE_UP			(WM_USER+121)
#define	WM_FILTER_MAIN_MOUSE_MOVE		(WM_USER+122)
#define	WM_FILTER_MAIN_KEY_DOWN			(WM_USER+123)
#define	WM_FILTER_MAIN_KEY_UP			(WM_USER+124)
#define	WM_FILTER_MAIN_MOVESIZE			(WM_USER+125)
#define FILTER_UPDATE_STATUS_ALL		0
#define FILTER_UPDATE_STATUS_TRACK		0x10000
#define FILTER_UPDATE_STATUS_CHECK		0x20000

//	�t�B���^DLL�p�\����
typedef struct {
	int			flag;
	int			x,y;
	TCHAR		*name;
	int			track_n;
	TCHAR		**track_name;
	int			*track_default;
	int			*track_s,*track_e;
	int			check_n;
	TCHAR		**check_name;
	int			*check_default;
	BOOL		(*func_proc)( FILTER *fp,FILTER_PROC_INFO *fpip );
	BOOL		(*func_init)( FILTER *fp );
	BOOL		(*func_exit)( FILTER *fp );
	BOOL		(*func_update)( FILTER *fp,int status );
	BOOL 		(*func_WndProc)( HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam,void *editp,FILTER *fp );
	int			*track,*check;
	void		*ex_data_ptr;
	int			ex_data_size;
	TCHAR		*information;
	BOOL		(*func_save_start)( FILTER *fp,int s,int e,void *editp );
	BOOL		(*func_save_end)( FILTER *fp,void *editp );
	EXFUNC		*exfunc;
	HWND		hwnd;
	HINSTANCE	dll_hinst;
	void		*ex_data_def;
	BOOL		(*func_is_saveframe)( FILTER *fp,void *editp,int saveno,int frame,int fps,int edit_flag,int inter );
	int			reserve[6];
} FILTER_DLL;

#define	MID_FILTER_BUTTON			12004

BOOL func_proc( FILTER *fp,FILTER_PROC_INFO *fpip );
BOOL func_init( FILTER *fp );
BOOL func_exit( FILTER *fp );
BOOL func_update( FILTER *fp,int status );
BOOL func_WndProc( HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam,void *editp,FILTER *fp );
BOOL func_save_start( FILTER *fp,int s,int e,void *editp );
BOOL func_save_end( FILTER *fp,void *editp );
BOOL func_is_saveframe( FILTER *fp,void *editp,int saveno,int frame,int fps,int edit_flag,int inter );


#endif
