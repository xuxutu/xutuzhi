#include "mycomplex.h"


ostream& operator << (ostream& os,const MyComplex& mc)
{

	return os<<"["<<mc.real()<<"]"<<"["<<mc.imag()<<"]";
}

inline
MyComplex operator + (const MyComplex& x,const MyComplex& y)//两个复数相加返回local变量
{
	
	return MyComplex(x.real()+y.real(),x.imag()+y.imag());
}

inline
MyComplex operator + (const MyComplex& x,double y)//复数+double
{
	return MyComplex(x.real()+y,x.imag());
} 

inline
MyComplex operator + (double x,const MyComplex& y)//double+复数
{

	return MyComplex(x+y.real(),y.imag());
}

inline
MyComplex& __doapl(MyComplex* ths,const MyComplex& r)
{
	ths->re += r.re;
	ths->im += r.im;

	return *ths;
}

inline
MyComplex& MyComplex::operator += (const MyComplex& r)
{
	return __doapl(this, r);
}


