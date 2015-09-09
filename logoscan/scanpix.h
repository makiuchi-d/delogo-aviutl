/*********************************************************************
* 	class ScanPixel
* 	各ピクセルのロゴ色・不透明度解析用クラス
* 
*********************************************************************/
#ifndef ___SCANPIX_H
#define ___SCANPIX_H


#include <windows.h>

#include "..\\filter.h"
#include "..\\logo.h"



class ScanPixel {
protected:
	short*     lst_y;
	short*     lst_cb;
	short*     lst_cr;
	short*     lst_bgy; 	// 輝度
	short*     lst_bgcb;	// 色差（青）
	short*     lst_bgcr;	// 色差（赤）
	int        n;		// サンプル枚数
	int        bufsize;

public:
	ScanPixel(void);
	~ScanPixel();

	int  AddSample(PIXEL_YC& ycp,PIXEL_YC& ycp_bg);
	int  AddSample(PIXEL& rgb,PIXEL& rgb_bg);
	int  EditSample(int num,PIXEL_YC& ycp,PIXEL_YC& ycp_bg);
	int  EditSample(int num,PIXEL& rgb,PIXEL rgb_bg);
	int  DeleteSample(int num);
	int  ClearSample(void);
	int  GetLGP(LOGO_PIXEL& plgp);
	int  GetAB_Y(double& A,double& B);
	int  GetAB_Cb(double& A,double& B);
	int  GetAB_Cr(double& A,double& B);
};


#endif
