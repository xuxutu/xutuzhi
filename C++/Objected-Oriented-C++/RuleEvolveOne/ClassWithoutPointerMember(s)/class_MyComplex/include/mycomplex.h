#ifndef __MYCOMPLEX__
#define __MYCOMPLEX__
#include <iostream>
using namespace std;

class MyComplex
{
public:
	MyComplex(double r=1,double i=1);  //声明时有默认值，定义时不可有默认值；参数列表只能在定义时写出；
	
	double real()const{return re;}
	double imag()const{return im;}

	MyComplex& operator += (const MyComplex&);

	friend MyComplex& __doapl(MyComplex* ths,const MyComplex& rhs);

private:
	double re;
	double im;
};

ostream& operator << (ostream& os,const MyComplex& rhs); 


//复数+复数
MyComplex operator + (const MyComplex& x,const MyComplex& y);
//复数+double
MyComplex operator + (const MyComplex& x,double y);
//double+复数
MyComplex operator + (double x,const MyComplex& y);

#endif
