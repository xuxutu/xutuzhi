#include "MyString.h"
#include <string.h>
#include <iostream>

using namespace std;

//默认构造函数
MyString::MyString(const char* c_str)
{
	cout<<"caling default constructor"<<endl;
	if(c_str)
	{
		m_data=new char[strlen(c_str)+1];
		strcpy(m_data,c_str);
	}
	else
	{
		m_data=new char[1];
		m_data[0]=0;
	}
	cout<<"caling default constructor"<<endl;
}

//拷贝构造函数
MyString::MyString(const MyString& str)
{
	m_data=new char[strlen(str.m_data)+1];
	strcpy(m_data,str.m_data);
	cout<<"calling copy constructor"<<endl;
}

//拷贝赋值函数
MyString& MyString::operator=(const MyString& str)
{
	//判断是否是自我赋值
	if(this==&str)
	{
		return *this;
	}
	
	//先干掉自己
	delete[] m_data;
	//分配内存
	m_data=new char[strlen(str.m_data)+1];
	strcpy(m_data,str.m_data);

	cout<<"calling copy assignment"<<endl;
}

MyString::~MyString()
{
	delete[] m_data;
	cout<<"calling destructor"<<endl;
}

ostream& operator<< (ostream& io,MyString& str)
{
	return io<<str.get_c_str();
}
