#ifndef __MYSTRING__
#define __MYSTRING__
#include <iostream>
#include <string.h>
using namespace std;
class MyString
{
public:
	MyString(const char* c_str);
	
	MyString(const MyString& str);

	MyString& operator = (const MyString& str);

	~MyString(){delete[] m_data;}
	
	char* get_c_str()const{return m_data;}

	char& operator [] (int size)
	{
		return m_data[size];
	}

	char operator [] (int size)const	
	{
		return m_data[size];
	}
private:
	char* m_data;
};

ostream& operator << (ostream& os,const MyString& str);


#endif







