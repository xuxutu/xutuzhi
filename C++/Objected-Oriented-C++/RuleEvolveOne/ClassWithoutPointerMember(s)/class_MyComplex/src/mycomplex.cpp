#include "mycomplex.h"

ostream& operator << (ostream& os,const MyComplex& rhs)
{

	return os<<"["<<rhs.real()<<"]"<<"["<<rhs.imag()<<"]";
}


MyComplex::MyComplex(double r,double i) : re(r),im(i) 
{
	
	//cout<<r<<"--"<<i<<endl; 

} 

MyComplex& __doapl(MyComplex* ths,const MyComplex& rhs)
{
	ths->re += rhs.re;
	ths->im += rhs.im;
	return *ths; 
}

MyComplex& MyComplex::operator += (const MyComplex& rhs)
{

	return __doapl(this,rhs);
}

MyComplex operator + (const MyComplex& x,const MyComplex& y)
{
	return MyComplex(x.real()+y.real(),x.imag()+y.imag());
}

MyComplex operator + (const MyComplex& x,double y)
{
	return MyComplex(x.imag()+y,x.real());
}

MyComplex operator + (double x,const MyComplex& y)
{
	return MyComplex(x+y.real(),y.imag());
}
