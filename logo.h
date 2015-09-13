/*====================================================================
*	���S�p�^�[��			logo.h
* 
* [���S�f�[�^�t�@�C���\��]
* 
* 	"<logo file x.xx>"	// �t�@�C���w�b�_������F�o�[�W�������(28byte)
* 	+----
* 	|	�t�@�C���Ɋ܂܂�郍�S�f�[�^�̐�(4byte, BigEndian)
* 	+----
* 	|	LOGO_HEADER		// �f�[�^�w�b�_
* 	+----
* 	|
* 	:	LOGO_PIXEL[h*w]	// �s�N�Z�����F�T�C�Y��LOGO_HEADER��w,h����Z�o
* 	:
* 	+----
* 	|	LOGO_HEADER
* 	+----
* 	|
* 	:	LOGO_PIXEL[h*w]
* 	:
* 
*===================================================================*/
#ifndef ___LOGO_H
#define ___LOGO_H

/* ���S�w�b�_������ */
#define LOGO_FILE_HEADER_STR "<logo data file ver0.1>\0\0\0\0\0"
#define LOGO_FILE_HEADER_STR_SIZE  28

/*--------------------------------------------------------------------
*	LOGO_FILE_HEADER �\����
*		�t�@�C���w�b�_�D
*		�o�[�W�������Ɗ܂܂��f�[�^��
*-------------------------------------------------------------------*/
typedef struct {
	char str[LOGO_FILE_HEADER_STR_SIZE];
	union{
		unsigned long l;
		unsigned char c[4];
	} logonum;
} LOGO_FILE_HEADER;

#define SWAP_ENDIAN(x) (((x&0xff)<<24)|((x&0xff00)<<8)|((x&0xff0000)>>8)|((x&0xff000000)>>24))

/* �s�����x�ő�l */
#define LOGO_MAX_DP   1000

/* ���S���ő啶�����i�I�[\0�܂ށj */
#define LOGO_MAX_NAME 32

/*--------------------------------------------------------------------
*	LOGO_HEADER �\����
*		���S�̊�{�I�ȏ����L�^
*-------------------------------------------------------------------*/
typedef struct {
	char     name[LOGO_MAX_NAME]; 	/* ����                   */
	short    x, y;      			/* ��{�ʒu               */
	short    h, w;      			/* ���S�����E��           */
	short    fi, fo;    			/* �f�t�H���g��FadeIn/Out */
	short    st, ed;    			/* �f�t�H���g�̊J�n��I��  */
} LOGO_HEADER;

/*--------------------------------------------------------------------
*	LOGO_PIXEL �\����
*		���S�̊e�s�N�Z�����Ƃ̏����L�^
*-------------------------------------------------------------------*/
typedef struct {
	short dp_y;		/* �s�����x�i�P�x�j            */
	short y;		/* �P�x              0�`4096   */
	short dp_cb;	/* �s�����x�i�j              */
	short cb;		/* �F���i�j    -2048�`2048   */
	short dp_cr;	/* �s�����x�i�ԁj              */
	short cr;		/* �F���i�ԁj    -2048�`2048   */
} LOGO_PIXEL;

/*--------------------------------------------------------------------
*	���S�f�[�^�̃T�C�Y�i�w�b�_�����j
*-------------------------------------------------------------------*/
#define LOGO_PIXELSIZE(ptr)  \
	(((LOGO_HEADER *)ptr)->h*((LOGO_HEADER *)ptr)->w*sizeof(LOGO_PIXEL))

/*--------------------------------------------------------------------
*	���S�f�[�^�S�̂̃T�C�Y
*-------------------------------------------------------------------*/
#define LOGO_DATASIZE(ptr) (sizeof(LOGO_HEADER)+LOGO_PIXELSIZE(ptr))

#endif
