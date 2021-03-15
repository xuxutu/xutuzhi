#include "string.h"
#include <cstring>
#include< iostream>

using namespace std;

inline
String::String(const char* cstr)
{
	if(cstr)
	{
		m_data=new char[strlen(cstr)+1];
        strcpy(m_data,cstr);
	}
	else
	{
		m_data=new char[1];
        m_data[0]='\0';
	}

	cout<<"default constructor"<<endl;
}


inline 
String::String(const String& str)
{
	m_data=new char[strlen(str.m_data)+1];
    strcpy(m_data,str.m_data);
	cout<<"copy constructor"<<endl;
}

String& String::operator=(const String& str)
{
	if(this==&str)//检测自我赋值；此处并非只是效率提升，如果是同一指针，先释放内存后，无法再拷贝。
	{
		return *this;
	}

   delete [] m_data;//1.销毁自己
   m_data=new char[strlen(str.m_data)+1];//2.创造自己
   strcpy(m_data,str.m_data);//3.拷贝

   cout<<"copy assignment constructor"<<endl;	
	
   return *this;
}


inline
String::~String()
{

	delete[] m_data;

}


ostream& operator<<(ostream& os,const String& str)
{

	os<<str.get_c_str();
    return os;
}

