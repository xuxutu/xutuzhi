#include "myfraction.h"
#include <iostream>
using namespace std;

int main()
{
	MyFraction f1(8);
	double a = 0.75 + f1;//f1 -> double
	/*	
		1.没有全局函数operator+(double,MyFraction),skip
		2.0.75->MyFraction,ok（构造函数自带隐式转型）		
	*/
	MyFraction f2=4+f1; //f1 -> double 
	/*
		编译器转变：4——>MyFraction,发现没有全局operator+，skip
				    MyFraction——>double,ok  			
	*/
	
	//MyFraction f2=f1+4; 
	/*
		1.f1 -> double，ok，默认构造的隐式转换，ok
		2.4 -> f1,ok(默认构造的隐式转换),f1+f1,ok	
	*/
	//cout<< f2.getNum() <<endl;
	

	return 0;
}

