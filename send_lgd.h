/*====================================================================
* 	共通ウィンドウメッセージ		send_lgd.h
*===================================================================*/
#ifndef ___SEND_LGD_H
#define ___SEND_LGD_H


// 送信側：	RegisterWindowMessage()で登録
//			get_filterp()でロゴ消しフィルタのfpを取得（フィルタ名で判別）
//			fp->hwndへSendMessage()：wParamにロゴデータへのポインタ。lParam=0

// 受信側：	RegisterWindowMessage()で登録
//			このメッセージが送られた時受信処理

const char wm_send_logo_data[] = "WM_SEND_LOGO_DATA";

// ロゴフィルタ名
#define LOGO_FILTER_NAME "透過性ロゴ"

#endif
