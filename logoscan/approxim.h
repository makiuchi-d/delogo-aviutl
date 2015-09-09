/*********************************************************************
* 	��A�����������̎擾�i�ŏ����@�j   approxim.h
* 
* N�̗v�f(Xi,Yi)-> Y = a * X + b
* 
* a = (N*��XiYi - ��Xi*��Yi)/(N*��Xi^2 -(��Xi)^2)
* b = (��(Xi^2)*��Yi - ��Xi*��(Xi*Yi))/(N*��Xi^2 -(��Xi)^2)
* 
*********************************************************************/
#ifndef ___APPROXIM_H
#define ___APPROXIM_H

/*====================================================================
* 	approxim_line()	��A�����֐��擾
* 		x: X���̔z��
* 		y: Y���̔z��
* 		n: �z��̗v�f��
* 		a: �X���o�b�t�@�ւ̃|�C���^
* 		b: �ؕЃo�b�t�@�ւ̃|�C���^
*===================================================================*/
template <class T>
void approxim_line(T* x,T* y,int n,double& a,double& b)
{
	double sum_x,sum_y,sum_x2,sum_xy;
	double temp;

	sum_x  = sum(x,n);	// ��Xi
	sum_y  = sum(y,n);	// ��Yi
	sum_x2 = sq_sum(x,x,n);	// ��(X^2)
	sum_xy = sq_sum(x,y,n);	// ��(X*Y)

	// �O�ł̏��Z���
	temp = (double)n*sum_x2 - sum_x*sum_x;
	if(temp==0) throw "Can't get approxim line";

	a = ((double)n*sum_xy - sum_x*sum_y)/temp;
	b = (sum_x2*sum_y - sum_x*sum_xy)/temp;
}

/*--------------------------------------------------------------------
* 	sum()	��X
* 		x: �z��
* 		n: �v�f��
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
* 	sum_sq()	��(X*Y)
* 		x: �z��P
* 		y: �z��Q
* 		n: �v�f��
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
