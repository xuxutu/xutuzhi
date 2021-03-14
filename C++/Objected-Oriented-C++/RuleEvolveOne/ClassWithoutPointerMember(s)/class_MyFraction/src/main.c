#include "myfraction.h"
#include <iostream>
using namespace std;

int main()
{
	MyFraction mf1 (1, 2);
	MyFraction mf (1, 2);
	
	//mf = mf1 + 1; //true
	//mf = 1 + mf1;	//false 
	
	/*
		隐式默认构造出的对象不可调用成员函数，可以作为成员函数的参数
	*/

	//MyFraction mf2 = 0.5 + mf1;
	/*
		MyFraction mf2 = 0.5; ***只有当这样写时会触发隐式构造
	*/
	
	MyFraction mf2 = mf1 + 0.5;
	
	//cout<<mf2<<endl;

	return 0;
}
