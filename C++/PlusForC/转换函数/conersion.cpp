#include<iostream>
using namespace std;


//转换函数
namespace sp1
{
	class Fraction
	{
	public:
		Fraction(int num,int den=1) : m_numerator(num),m_denominator(den){}
		operator double()const //编译器会自动添加返回类型
		{
			return (double)(m_numerator/m_denominator);
		}
	private:
		int m_numerator;//分子
		int m_denominator;//分母
	};
}

//non-explicit-one-argument ctor
namespace sp2
{	
    class Fraction
    {
    public://explict能够防止如下隐式转换
        Fraction(int num,int den=1) : m_numerator(num),m_denominator(den){}//non-explicit-one-argument ctor 
        Fraction operator+(const Fraction& f)
        {
            return Fraction(1,1);//暂且写成这样
        }
    private:
        int m_numerator;//分子
        int m_denominator;//分母
    };

}


int main()
{
	sp1::Fraction f(3,5);
    double d=4+f; //Fraction -> double
    cout<<d<<endl;

	sp2::Fraction f2(3,5);
    sp2::Fraction d2=f2+4; //4->Fraction
	return 0;
}
