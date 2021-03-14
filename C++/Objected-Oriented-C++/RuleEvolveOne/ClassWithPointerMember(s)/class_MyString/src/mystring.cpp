#include "mystring.h"

MyString::MyString(const char* c_str=0)
{
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

MyString::MyString(const MyString& str)
{
	m_data=new char[strlen(str.m_data)+1];
	strcpy(m_data,str.m_data);
	cout<<"calling copy constructor"<<endl;
}

MyString& MyString::operator = (const MyString& str)
{
	if(this == &str)
	{
		return *this;
	}
	else
	{
		delete[] m_data;
		m_data=new char[strlen(str.m_data)+1];
		strcpy(m_data,str.m_data);		
	}

	cout<<"calling copy assignment"<<endl;
}


ostream& operator << (ostream& os,const MyString& str)
{
	
	return os<<str.get_c_str();
}
