#include "myMyComplex.h"

inline
MyComplex	operator + (const MyComplex& x,const MyComplex& y)
{
	
	return MyComplex(x.rel+y.rel,x.img+y.img);
}

inline
MyComplex& __doapl(MyComplex* ths,const MyComplex& r)
{
	this->re += r.re;
	this->im += r.im;

	return *ths;
}

inline
MyComplex& MyComplex::operator += (const MyComplex& r)
{
	return __doapl(this, r);
}
