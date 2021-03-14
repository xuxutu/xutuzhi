#ifndef __MYFRACTION__
#define __MYFRACYION__
#include <iostream>
using namespace std;

class MyFraction
{
public:
	/*explicit*/ MyFraction(int nutor,int detor=1) : numerator(nutor), denominator(detor){cout<<"call constructor!"<<endl;}//分母默认是1，符合实际的认识
/*
	operator double ()const	//转换函数
	{
		cout<<"call double conversion!"<<endl;
		return (double)numerator/(double)denominator;
	}
*/
	MyFraction& operator + (const MyFraction& mf)
	{
		return *this;//暂不实现具体的分加法
	}

	double getValue()const
	{
		return (double)numerator/(double)denominator;		
	}

private:
	int	numerator;	 //分子	
	int denominator; //分母	
};

#endif
