/*********************************************************************
* 	ñA¼üûö®Ìæ¾iÅ¬ñæ@j   approxim.h
* 
* NÂÌvf(Xi,Yi)-> Y = a * X + b
* 
* a = (N*°XiYi - °Xi*°Yi)/(N*°Xi^2 -(°Xi)^2)
* b = (°(Xi^2)*°Yi - °Xi*°(Xi*Yi))/(N*°Xi^2 -(°Xi)^2)
* 
*********************************************************************/
#ifndef ___APPROXIM_H
#define ___APPROXIM_H

/*====================================================================
* 	approxim_line()	ñA¼üÖæ¾
* 		x: X²Ìzñ
* 		y: Y²Ìzñ
* 		n: zñÌvf
* 		a: X«obt@ÖÌ|C^
* 		b: ØÐobt@ÖÌ|C^
* 	Ôpl
* 		¬÷Ftrue	¸sFfalse
*===================================================================*/
template <class T>
bool approxim_line(T* x,T* y,int n,double& a,double& b)
{
	double sum_x,sum_y,sum_x2,sum_xy;
	double temp;

	sum_x  = sum(x,n);	// °Xi
	sum_y  = sum(y,n);	// °Yi
	sum_x2 = sq_sum(x,x,n);	// °(X^2)
	sum_xy = sq_sum(x,y,n);	// °(X*Y)

	// OÅÌ¤Zñð
	temp = (double)n*sum_x2 - sum_x*sum_x;
	if(temp==0.0) return false;

	a = ((double)n*sum_xy - sum_x*sum_y)/temp;
	b = (sum_x2*sum_y - sum_x*sum_xy)/temp;

	return true;
}

/*--------------------------------------------------------------------
* 	sum()	°X
* 		x: zñ
* 		n: vf
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
* 	sum_sq()	°(X*Y)
* 		x: zñP
* 		y: zñQ
* 		n: vf
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
