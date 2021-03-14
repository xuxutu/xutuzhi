#ifndef __MYFRACTION__
#define __MYFRACTION__

class MyFraction
{
public:
	MyFraction(int num, int den = 1);

	operator double()const;		//编译器自动添加返回类型double
	
	int getNum()const
	{
		return m_numerator;
	}

	MyFraction operator +(const MyFraction& mf)
	{
		return MyFraction(0);//写0测试
	}

private:
	int m_numerator;//分子
	int m_denominator;//分母	
};


#endif 
