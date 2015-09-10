/*********************************************************************
* 	回帰直線方程式の取得（最小二乗法）   approxim.h
* 
* N個の要素(Xi,Yi)-> Y = a * X + b
* 
* a = (N*ΣXiYi - ΣXi*ΣYi)/(N*ΣXi^2 -(ΣXi)^2)
* b = (Σ(Xi^2)*ΣYi - ΣXi*Σ(Xi*Yi))/(N*ΣXi^2 -(ΣXi)^2)
* 
*********************************************************************/
#ifndef ___APPROXIM_H
#define ___APPROXIM_H

/*====================================================================
* 	approxim_line()	回帰直線関数取得
* 		x: X軸の配列
* 		y: Y軸の配列
* 		n: 配列の要素数
* 		a: 傾きバッファへのポインタ
* 		b: 切片バッファへのポインタ
* 	返却値
* 		成功：true	失敗：false
*===================================================================*/
template <class T>
bool approxim_line(T* x,T* y,int n,double& a,double& b)
{
	double sum_x,sum_y,sum_x2,sum_xy;
	double temp;

	sum_x  = sum(x,n);	// ΣXi
	sum_y  = sum(y,n);	// ΣYi
	sum_x2 = sq_sum(x,x,n);	// Σ(X^2)
	sum_xy = sq_sum(x,y,n);	// Σ(X*Y)

	// ０での商算回避
	temp = (double)n*sum_x2 - sum_x*sum_x;
	if(temp==0.0) return false;

	a = ((double)n*sum_xy - sum_x*sum_y)/temp;
	b = (sum_x2*sum_y - sum_x*sum_xy)/temp;

	return true;
}

/*--------------------------------------------------------------------
* 	sum()	ΣX
* 		x: 配列
* 		n: 要素数
*-------------------------------------------------------------------*/
template <class T>
static double sum(T* x,int n)
{
	int i;
	double a = 0.0;

	for(i=0;i<n;i++)
		a += x[i];

	return a;
}

/*--------------------------------------------------------------------
* 	sum_sq()	Σ(X*Y)
* 		x: 配列１
* 		y: 配列２
* 		n: 要素数
*-------------------------------------------------------------------*/
template <class T>
static double sq_sum(T* x,T* y,int n)
{
	int i;
	double a = 0.0;

	for(i=0;i<n;i++)
		a += x[i] * y[i];

	return a;
}


#endif
