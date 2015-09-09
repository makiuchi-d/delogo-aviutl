/*********************************************************************
* 	class RectList
* 		範囲リスト管理クラス
* 
*********************************************************************/
#ifndef ___RECTLIST_H
#define ___RECTLIST_H

class XYWH {
protected:
public:
	short x;
	short y;
	short w;
	short h;

	XYWH(void)
		{ x=y=w=h=-1; }
	XYWH(XYWH& r)
		{ x=r.x; y=r.y; w=r.w; h=r.h; }
	XYWH(int nx,int ny,int nw,int nh)
		{ x=nx; y=ny; w=nw; h=nh; }
};
#endif
