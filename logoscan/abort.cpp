/*====================================================================
* 	経過表示・中断ダイアログ
* 
* 2003
* 	06/22:	中断できるようにしよう！ついでに経過表示もこっちで。
* 	07/02:	ようやく完成
* 
*===================================================================*/
#include <windows.h>
#include <commctrl.h>
#include "abort.h"
#include "resource.h"
#include "..\filter.h"


#define tLOGOX   0
#define tLOGOY   1
#define tLOGOW   2
#define tLOGOH   3
#define tTHY     4


class XYWH {
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

bool  Cal_BGcolor(PIXEL_YC&,PIXEL_YC*,XYWH&,int,int);
int   comp_short(const void* x,const void* y);
short med_average(short* s,int n);
void  CreateLogoData(AbortDlgParam* p,HWND hdlg);//FILTER* fp,ScanPixel*& sp,void*&);

/*====================================================================
* 	AbortDlgProc()		コールバックプロシージャ
*===================================================================*/
BOOL CALLBACK AbortDlgProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const  int IDD_TIMER = 4301;
	static int examine;	// スキャン済み
	static int useable;	// 有効サンプル
	static AbortDlgParam* p;
	static bool abort;

	switch(msg){
		case WM_INITDIALOG:
			p = (AbortDlgParam*)lParam;
			SetTimer(hdlg,IDD_TIMER,1,NULL);
			SetDlgItemInt(hdlg,IDC_ALLF,p->e-p->s+1,false);
			examine = useable = 0;
			abort = false;
			break;

		case WM_CLOSE:
			EndDialog(hdlg,0);
			break;


		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDC_ABORT:
					abort = true;
					break;
			}
			break;

		case WM_TIMER:	// 読み取り開始
			KillTimer(hdlg,IDD_TIMER);

			//----------------------------------------------- ロゴ解析
			try{
				SendDlgItemMessage(hdlg,IDC_PROGRESS,PBM_SETRANGE,0,MAKELPARAM(0,p->e-p->s+1));

				while(p->s+examine <= p->e && !abort){
					// pump windows message
					MSG message;
					while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
						TranslateMessage(&message);
						DispatchMessage(&message);
					}

					SendDlgItemMessage(hdlg,IDC_PROGRESS,PBM_SETPOS,examine,0);
					SetDlgItemInt(hdlg,IDC_USEABLE,useable,FALSE);
					SetDlgItemInt(hdlg,IDC_EXAMF,examine,FALSE);

					// 画像取得
					PIXEL_YC* pix = p->fp->exfunc->get_ycp_filtering_cache_ex(p->fp,p->editp,p->s+examine,NULL,NULL);

					// 背景平均値計算
					PIXEL_YC bg;
					XYWH     xywh(p->x,p->y,p->w,p->h);
					if(Cal_BGcolor(bg,pix,xywh,p->max_w,p->t)){
						// 単一背景のときサンプルをセットする
						useable++;
						SendMessage(p->fp->hwnd,WM_SP_DRAWFRAME,0,p->s+examine);
						// ロゴサンプルセット
						for(int i=0;i<xywh.h;i++){
							for(int j=0;j<xywh.w;j++){
								PIXEL_YC ptr;
								ptr = pix[(xywh.y+i)*p->max_w + xywh.x+j];
								p->sp[i*xywh.w+j].AddSample(ptr,bg);	// X軸:背景
							}
						}
					}
					examine++;

				}	// end of while

				// ロゴデータ作成
				CreateLogoData(p,hdlg);
			}
			catch(const char* str){
				p->errstr = str;
			}
			EndDialog(hdlg,0);

			break;
	}

	return 0;
}

/*--------------------------------------------------------------------
*	ロゴデータを作成
*-------------------------------------------------------------------*/
void CreateLogoData(AbortDlgParam* p,HWND hdlg)
{
	SetDlgItemText(hdlg,IDC_STATUS,"ロゴデータ構築中...");

	// ロゴヘッダ作成（名称以外）
	LOGO_HEADER lgh;
	ZeroMemory(&lgh,sizeof(LOGO_HEADER));
	lgh.x = p->x;//fp->track[tLOGOX];
	lgh.y = p->y;//fp->track[tLOGOY];
	lgh.w = p->w;//fp->track[tLOGOW];
	lgh.h = p->h;//fp->track[tLOGOH];

	// ロゴデータ領域確保
	*p->data = (void*) new char[LOGO_DATASIZE(&lgh)];
	if(p->data==NULL) throw "メモリ確保できませんでした";
	*((LOGO_HEADER*)*p->data) = lgh;	// ヘッダコピー

	LOGO_PIXEL* lgp;
	lgp = (LOGO_PIXEL*) ((LOGO_HEADER*)*p->data+1);

	SendDlgItemMessage(hdlg,IDC_PROGRESS,PBM_SETRANGE,0,MAKELPARAM(0,lgh.w*lgh.h-1));
	
	for(int i=0;i<lgh.w*lgh.h;i++){
		SendDlgItemMessage(hdlg,IDC_PROGRESS,PBM_SETPOS,i,0);
		p->sp[i].GetLGP(lgp[i]);
	}
}


/*--------------------------------------------------------------------
*	背景色計算
*-------------------------------------------------------------------*/
bool Cal_BGcolor(PIXEL_YC& r,PIXEL_YC* pix,XYWH& xywh,int w,int thy)
{
	short* y;	// 背景色配列
	short* cb;
	short* cr;
	int i,n;

	n = 0;

	// (幅+高さ+2)*2
	y  = new short[(xywh.w+xywh.h+2)*2];
	cb = new short[(xywh.w+xywh.h+2)*2];
	cr = new short[(xywh.w+xywh.h+2)*2];

	pix += xywh.x-1 + (xywh.y-1)*w;	// X-1,Y-1に移動

	// 横線（上）合計
	for(i=0;i<=xywh.w+1;i++){
		y[n]  = pix->y;
		cb[n] = pix->cb;
		cr[n] = pix->cr;
		n++;
		pix++;
	}
	pix += w - i;	// 次の行へ
	// 縦線
	for(i=2;i<=xywh.h+1;i++){
		// 左線
		y[n]  = pix->y;
		cb[n] = pix->cb;
		cr[n] = pix->cr;
		n++;
		// 右線
		y[n]  = pix[xywh.w+1].y;
		cb[n] = pix[xywh.w+1].cb;
		cr[n] = pix[xywh.w+1].cr;
		n++;

		pix += w;	// 次の行へ
	}
	// 横線（下）合計
	for(i=0;i<=xywh.w+1;i++){
		y[n]  = pix->y;
		cb[n] = pix->cb;
		cr[n] = pix->cr;
		n++;
		pix++;
	}

	// ソートして真中らへんを平均
	r.y  = med_average(y,n);
	r.cb = med_average(cb,n);
	r.cr = med_average(cr,n);

	bool ret = true;	// 返却値

	// 最小と最大が閾値以上離れている場合、単一色でないと判断
	if((abs(y[0] - y[n-1])>thy*8) ||
	   (abs(cb[0]-cb[n-1])>thy*8) ||
	   (abs(cr[0]-cr[n-1])>thy*8))
			ret = false;

	delete[] y;
	delete[] cb;
	delete[] cr;

	return ret;
}

/*--------------------------------------------------------------------
*	short型比較関数（qsort用）
*-------------------------------------------------------------------*/
int comp_short(const void* x,const void* y)
{
	return (*(const short*)x - *(const short*)y);
}

/*--------------------------------------------------------------------
*	ソートして真中らへんを平均
*-------------------------------------------------------------------*/
short med_average(short* s,int n)
{
	double t  =0.0;
	int    nn =0;

	// ソートする
	qsort(s,n,sizeof(short),comp_short);

	// 真中らへんを平均
	for(int i=n/3;i<n-(n/3);i++,nn++)
		t += s[i];

	t = t / nn + 0.5;

	return ((short)t);
}


//*/
