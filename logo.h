/*====================================================================
*	���S�p�^�[��			logo.h
* 
* [�t�@�C���\��]�i�b��Łj
* 
* 	"logo file x.xx\n"	// �t�@�C���w�b�_������F�o�[�W�������Ƃ�(31byte)
* 	+----
* 	|	�t�@�C���Ɋ܂܂�郍�S�f�[�^�̐�(1byte)
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

/* ���S�t�@�C���w�b�_
* 	31BYTE�̕�����
* 	�f�[�^�� 1BYTE
*/
#define LOGO_FILE_HEADER "<logo data file ver0.1>\0\0\0\0\0\0\0\0\0\0\0"
#define LOGO_FILE_HEADER_STR_SIZE  31


/* ���S�f�[�^�ő�T�C�Y�F
* 		���S�f�[�^���v���t�@�C���ɕۑ����Ȃ��悤�ɂ������߁A
* 		�T�C�Y�����͎����Ȃ��Ȃ���
*/
#define LOGO_MAXPIXEL (21840)
#define LOGO_MAXSIZE  (0x40000)

/* �s�����x�ő�l */
#define LOGO_MAX_DP   1000

/* ���S���ő啶�����i�I�[\0�܂ށj */
#define LOGO_MAX_NAME 32

/*--------------------------------------------------------------------
*	LOGO_HEADER �\����
*		���S�̊�{�I�ȏ����L�^
*-------------------------------------------------------------------*/
typedef struct {
	char     name[LOGO_MAX_NAME]; 	// ����
	short    x, y;      			// ��{�ʒu
	short    h, w;      			// ���S�����E��
	int      reserve[2];			// �g���p�ɗ\��
} LOGO_HEADER;

/*--------------------------------------------------------------------
*	LOGO_PIXEL �\����
*		���S�̊e�s�N�Z�����Ƃ̏����L�^
*-------------------------------------------------------------------*/
typedef struct {
	short dp_y;		// �s�����x�i�P�x�j
	short y;		// �P�x
	short dp_cb;	// �s�����x�i�j
	short cb;		// �F���i�j
	short dp_cr;		// �s�����x�i�ԁj
	short cr;		// �F���i�ԁj
} LOGO_PIXEL;


/*--------------------------------------------------------------------
*	���S�f�[�^�S�̂̃T�C�Y
*-------------------------------------------------------------------*/
#define LOGO_DATASIZE(ptr)  \
	(sizeof(LOGO_HEADER)+((LOGO_HEADER *)ptr)->h*((LOGO_HEADER *)ptr)->w*sizeof(LOGO_PIXEL))

/*--------------------------------------------------------------------
*	���S�f�[�^�̃T�C�Y�i�w�b�_�����j
*-------------------------------------------------------------------*/
#define LOGO_PIXELSIZE(ptr)  \
	(((LOGO_HEADER *)ptr)->h*((LOGO_HEADER *)ptr)->w*sizeof(LOGO_PIXEL))


#endif
