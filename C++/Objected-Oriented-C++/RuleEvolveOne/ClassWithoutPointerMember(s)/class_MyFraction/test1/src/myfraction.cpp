#include "myfraction.h"
#include <iostream>
using namespace std;

MyFraction::MyFraction(int num, int den) : m_numerator(num), m_denominator(den)
{
	cout<<"calling default constructor"<<endl;
}

MyFraction::operator double()const
{
	cout<<"calling double() convertion!"<<endl;
	return (double)m_numerator / (double)m_denominator;
}


