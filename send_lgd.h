/*====================================================================
* 	���ʃE�B���h�E���b�Z�[�W		send_lgd.h
*===================================================================*/
#ifndef ___SEND_LGD_H
#define ___SEND_LGD_H


// ���M���F	RegisterWindowMessage()�œo�^
//			get_filterp()�Ń��S�����t�B���^��fp���擾�i�t�B���^���Ŕ��ʁj
//			fp->hwnd��SendMessage()�FwParam�Ƀ��S�f�[�^�ւ̃|�C���^�BlParam=0

// ��M���F	RegisterWindowMessage()�œo�^
//			���̃��b�Z�[�W������ꂽ����M����

const char wm_send_logo_data[] = "WM_SEND_LOGO_DATA";

// ���S�t�B���^��
#define LOGO_FILTER_NAME "���ߐ����S"

#endif
