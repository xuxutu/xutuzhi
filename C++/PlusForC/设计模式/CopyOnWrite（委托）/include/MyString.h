#ifndef __STRING__
#define __STRING__
#include <iostream>
#include <string.h>
using namespace std;

class MyString
{
public:
	MyString(const char* c_str=0);
	MyString(const MyString& str);
	MyString& operator=(const MyString& str); 
	~MyString();
	char* get_c_str()const{return m_data;}

	char& operator[](int size)		//写
	{
		cout<<"write!"<<endl;
		return m_data[size];	
	}
	
	char operator[](int size)const	//读
	{
		cout<<"read"<<endl;
		return m_data[size];
	}

private:
	char* m_data;	
};

ostream& operator<< (ostream& io,MyString& str);


#endif 
