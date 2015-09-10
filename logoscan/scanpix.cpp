/*********************************************************************
* 	class ScanPixel
* 	�e�s�N�Z���̃��S�F�E�s�����x��͗p�N���X
* 
* 		operator new[](size_t,T* p) ���g��������Ɨ�����B
* 		ScanPixel::Alloc���ŏ��Ɏg�����Ƃŉ���ł���B(2003/05/10)
* 
* 								(�ŏI�X�V�F2003/05/10)
*********************************************************************/
#include <windows.h>
#include "..\\filter.h"
#include "..\\logo.h"
#include "approxim.h"
#include "scanpix.h"


// �G���[���b�Z�[�W
const char* CANNOT_MALLOC = "���������m�ۂł��܂���ł���";

#define DP_RANGE 0x3FFF

/*--------------------------------------------------------------------
* 	RGBtoYCbCr()
*-------------------------------------------------------------------*/
inline void RGBtoYCbCr(PIXEL_YC& ycp,PIXEL& rgb)
{
//	RGB -> YCbCr
//	Y  =  0.2989*Red + 0.5866*Green + 0.1145*Blue
//	Cb = -0.1687*Red - 0.3312*Green + 0.5000*Blue
//	Cr =  0.5000*Red - 0.4183*Green - 0.0816*Blue
//	(rgb�����ꂼ��0�`1�͈̔͂̎��A
//	Y:0�`1�ACb:-0.5�`0.5�ACr:-0.5�`0.5)
//
//	AviUtl�v���O�C���ł́A
//		Y  :     0 �` 4096
//		Cb : -2048 �` 2048
//		Cr : -2048 �` 2048
	ycp.y  = (double) 0.2989*4096*rgb.r + 0.5866*4096*rgb.g + 0.1145*4096*rgb.b +0.5;
	ycp.cb = (double)-0.1687*4096*rgb.r - 0.3312*4096*rgb.g + 0.5000*4096*rgb.b +0.5;
	ycp.cr = (double) 0.5000*4096*rgb.r - 0.4183*4096*rgb.g - 0.0816*4096*rgb.b +0.5;
}

/*--------------------------------------------------------------------
* 	�b���C�u������realloc�̑���
* 		T* = new (T*) T[]
*-------------------------------------------------------------------*/
template <class T>
inline void* operator new[](size_t x,T* p)
{
	T* temp;
	if(x)
		temp = new T[x/sizeof(T)];	// �V�̈�
	else
		temp = NULL;

	if(p){
		if(temp)
			memcpy(temp,p,x);	// ���̈悩��R�s�[
		delete[] p;	// ���̈�J��
	}

	return temp;	// �V�̈��Ԃ�
}

/*--------------------------------------------------------------------
* 	Abs()	��Βl
*-------------------------------------------------------------------*/
template <class T>
inline T Abs(T x){
	return ((x>0) ? x : -x);
}

/*====================================================================
* 	�R���X�g���N�^
*===================================================================*/
ScanPixel::ScanPixel(void)
{
	bufsize = 32;
	lst_y    = new short[bufsize];	//(short*)malloc(bufsize*sizeof(short));
	lst_cb   = new short[bufsize];	//(short*)malloc(bufsize*sizeof(short));
	lst_cr   = new short[bufsize];	//(short*)malloc(bufsize*sizeof(short));
	lst_bgy  = new short[bufsize];	//(short*)malloc(bufsize*sizeof(short));
	lst_bgcb = new short[bufsize];	//(short*)malloc(bufsize*sizeof(short));
	lst_bgcr = new short[bufsize];	//(short*)malloc(bufsize*sizeof(short));
	n = 0;

	// �������m�ێ��s
	if(lst_y==NULL || lst_cb==NULL || lst_cr==NULL ||
		lst_bgy==NULL || lst_bgcb==NULL || lst_bgcr==NULL)
			throw CANNOT_MALLOC;
}

/*====================================================================
* 	�f�X�g���N�^
*===================================================================*/
ScanPixel::~ScanPixel()
{
	bufsize = 0;
	if(lst_y)    delete[] lst_y;	//free(lst_y);
	if(lst_cb)   delete[] lst_cb;	//free(lst_cb);
	if(lst_cr)   delete[] lst_cr;	//free(lst_cr);
	if(lst_bgy)  delete[] lst_bgy;	//free(lst_bgy);
	if(lst_bgcb) delete[] lst_bgcb;	//free(lst_bgcb);
	if(lst_bgcr) delete[] lst_bgcr;	//free(lst_bgcr);
}

/*====================================================================
* 	Alloc()
* 		���炩���߃t���[���������������m��
* 		���łɂ���̈�̓N���A�����
*===================================================================*/
int ScanPixel::Alloc(unsigned int f)
{
	ScanPixel::~ScanPixel();

	bufsize = f;

	lst_y    = new short[f];
	lst_cb   = new short[f];
	lst_cr   = new short[f];
	lst_bgy  = new short[f];
	lst_bgcb = new short[f];
	lst_bgcr = new short[f];

		// �������m�ێ��s
	if(lst_y==NULL || lst_cb==NULL || lst_cr==NULL ||
		lst_bgy==NULL || lst_bgcb==NULL || lst_bgcr==NULL)
			throw CANNOT_MALLOC;

	return f;
}

/*====================================================================
* 	AddSample()
* 		�T���v�����o�b�t�@�ɉ�����
*===================================================================*/
// YCbCr�p
int ScanPixel::AddSample(PIXEL_YC& ycp,PIXEL_YC& ycp_bg)
{
	if(n>=bufsize){	// �o�b�t�@������Ȃ����T�C�Y�ύX
		bufsize += 32;
		lst_y    = new (lst_y)   short[bufsize];
		lst_cb   = new (lst_cb)  short[bufsize];
		lst_cr   = new (lst_cr)  short[bufsize];
		lst_bgy  = new (lst_bgy) short[bufsize];
		lst_bgcb = new (lst_bgcb)short[bufsize];
		lst_bgcr = new (lst_bgcr)short[bufsize];

		// �������m�ێ��s
		if(lst_y==NULL || lst_cb==NULL || lst_cr==NULL ||
			lst_bgy==NULL || lst_bgcb==NULL || lst_bgcr==NULL)
				throw CANNOT_MALLOC;
	}

	// �e�v�f�z��̖����ɉ�����
	lst_y[n]  = ycp.y;
	lst_cb[n] = ycp.cb;
	lst_cr[n] = ycp.cr;
	lst_bgy[n]  = ycp_bg.y;
	lst_bgcb[n] = ycp_bg.cb;
	lst_bgcr[n] = ycp_bg.cr;

	return (++n);
}

//--------------------------------------------------------------------
// RGB�p
int ScanPixel::AddSample(PIXEL& rgb,PIXEL& rgb_bg)
{
	PIXEL_YC ycp,ycp_bg;

	// RGB->YC2
	RGBtoYCbCr(ycp,rgb);
	RGBtoYCbCr(ycp_bg,rgb_bg);

	return AddSample(ycp,ycp_bg);
}

/*====================================================================
* 	EditSample()
* 		�T���v��������������
*===================================================================*/
// YCbCr�p
int ScanPixel::EditSample(unsigned int num,PIXEL_YC& ycp,PIXEL_YC& ycp_bg)
{
	if(num>=n)	// num�Ԗڂ̗v�f�����݂��Ȃ���
		return AddSample(ycp,ycp_bg);

	lst_y[num]  = ycp.y;
	lst_cb[num] = ycp.cb;
	lst_cr[num] = ycp.cr;
	lst_bgy[num]  = ycp_bg.y;
	lst_bgcb[num] = ycp_bg.cb;
	lst_bgcr[num] = ycp_bg.cr;

	return num;
}


//--------------------------------------------------------------------
// RGB�p
int ScanPixel::EditSample(unsigned int num,PIXEL& rgb,PIXEL rgb_bg)
{
	PIXEL_YC ycp,ycp_bg;

	// RGB->YC2
	RGBtoYCbCr(ycp,rgb);
	RGBtoYCbCr(ycp_bg,rgb_bg);

	return EditSample(num,ycp,ycp_bg);
}

/*====================================================================
* 	DeleteSample()
* 		�T���v�����폜����
*===================================================================*/
int ScanPixel::DeleteSample(unsigned int num)
{
	n--;

	if(n<=0)	// �T���v�����O�ȉ��ɂȂ�Ƃ�
		return ClearSample();

	if(n==num)	// �Ō�̃T���v�����폜���鎞
		return n;	// n�����炷����

	// ���̃T���v�������O�ɃR�s�[
	memcpy(&lst_y[num], &lst_y[num+1], (n-num)*sizeof(short));
	memcpy(&lst_cb[num],&lst_cb[num+1],(n-num)*sizeof(short));
	memcpy(&lst_cr[num],&lst_cr[num+1],(n-num)*sizeof(short));
	memcpy(&lst_bgy[num], &lst_bgy[num+1], (n-num)*sizeof(short));
	memcpy(&lst_bgcb[num],&lst_bgcb[num+1],(n-num)*sizeof(short));
	memcpy(&lst_bgcr[num],&lst_bgcr[num+1],(n-num)*sizeof(short));

	return n;
}

/*====================================================================
* 	ClearSample()
* 		�S�T���v�����폜����
*===================================================================*/
int ScanPixel::ClearSample(void)
{
	ScanPixel::~ScanPixel();

	bufsize = 32;
	lst_y    = new (lst_y)    short[bufsize];
	lst_cb   = new (lst_cb)   short[bufsize];
	lst_cr   = new (lst_cr)   short[bufsize];
	lst_bgy  = new (lst_bgy)  short[bufsize];
	lst_bgcb = new (lst_bgcb) short[bufsize];
	lst_bgcr = new (lst_bgcr) short[bufsize];

	// �������m�ێ��s
	if(lst_y==NULL || lst_cb==NULL || lst_cr==NULL ||
		lst_bgy==NULL || lst_bgcb==NULL || lst_bgcr==NULL)
			throw CANNOT_MALLOC;

	return n = 0;
}

/*====================================================================
* 	GetLGP()
* 		LOGO_PIXEL��Ԃ�
*===================================================================*/
int ScanPixel::GetLGP(LOGO_PIXEL& lgp)
{
	double A;
	double B;
	double temp;

	// �P�x
	GetAB_Y(A,B);
	if(A==1){	// 0�ł̏��Z���
		lgp.y = lgp.dp_y = 0;
	}
	else{
		temp = B / (1-A) +0.5;
		if(Abs(temp)<0x7FFF){
			// short�͈͓̔�
			lgp.y = temp;
			temp = ((double)1-A) * LOGO_MAX_DP +0.5;
			if(Abs(temp)>DP_RANGE || short(temp)==0)
				lgp.y = lgp.dp_y = 0;
			else
				lgp.dp_y = temp;
		}
		else
			lgp.y = lgp.dp_y = 0;
	}

	// �F��(��)
	GetAB_Cb(A,B);
	if(A==1){
		lgp.cb = lgp.dp_cb = 0;
	}
	else{
		temp = B / (1-A) +0.5;
		if(Abs(temp)<0x7FFF){
			// short�͈͓�
			lgp.cb = temp;
			temp = ((double)1-A) * LOGO_MAX_DP +0.5;
			if(Abs(temp)>DP_RANGE || short(temp)==0)
				lgp.cb = lgp.dp_cb = 0;
			else
				lgp.dp_cb = temp;
		}
		else
			lgp.cb = lgp.dp_cb = 0;
	}

	// �F��(��)
	GetAB_Cr(A,B);
	if(A==1){
		lgp.cr = lgp.dp_cr = 0;
	}
	else {
		temp = B / (1-A) +0.5;
		if(Abs(temp)<0x7FFF){
			// short�͈͓�
			lgp.cr = temp;
			temp = ((double)1-A) * LOGO_MAX_DP + 0.5;
			if(Abs(temp)>DP_RANGE || short(temp)==0)
				lgp.cr = lgp.dp_cr = 0;
			else
				lgp.dp_cr = temp;
		}
		else
			lgp.cr = lgp.dp_cr = 0;
	}

	return n;
}

/*====================================================================
* 	GetAB_?()
* 		��A�����̌X���ƐؕЂ�Ԃ�
*===================================================================*/
int ScanPixel::GetAB_Y(double& A,double& B)
{
	double A1,A2;
	double B1,B2;

	// XY����ւ������̗����ŕ��ς����
	// �w�i��X��
	approxim_line(lst_bgy,lst_y,n,A1,B1);
	// �w�i��Y��
	approxim_line(lst_y,lst_bgy,n,A2,B2);

	A = (A1+(1/A2))/2;	// �X���𕽋�
	B = (B1+(-B2/A2))/2;	// �ؕЂ�����

	return n;
}
int ScanPixel::GetAB_Cb(double& A,double& B)
{
	double A1,A2;
	double B1,B2;

	approxim_line(lst_bgcb,lst_cb,n,A1,B1);
	approxim_line(lst_cb,lst_bgcb,n,A2,B2);

	A = (A1+(1/A2))/2;	// �X���𕽋�
	B = (B1+(-B2/A2))/2;	// �ؕЂ�����

	return n;
}
int ScanPixel::GetAB_Cr(double& A,double& B)
{
	double A1,A2;
	double B1,B2;

	approxim_line(lst_bgcr,lst_cr,n,A1,B1);
	approxim_line(lst_cr,lst_bgcr,n,A2,B2);

	A = (A1+(1/A2))/2;	// �X���𕽋�
	B = (B1+(-B2/A2))/2;	// �ؕЂ�����

	return n;
}
