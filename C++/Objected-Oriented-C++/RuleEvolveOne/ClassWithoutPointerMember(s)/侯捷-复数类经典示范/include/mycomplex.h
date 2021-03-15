#ifndef __MYCOMPLEX__
#define __MyCOMPLEX__
#include <iostream>
using namespace std;
class MyComplex	
{
public:
	MyComplex(double r=0,double i=0) : re(r),im(i){}
		
	MyComplex& operator += (const MyComplex& r);

	double real()const {return re;}
	double imag()const {return im;}

private:
	double re; //实
	double im; //虚
	
	friend MyComplex& __doapl(MyComplex*,const MyComplex&);//友元函数
};
	
ostream& operator << (ostream& os,const MyComplex& mc);
#endif
