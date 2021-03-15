#include <iostream>
#include "MyString.h"
#include <string.h>
using namespace std;

int main(void)
{
	MyString str1 ("hello");
	MyString str2(str1);
	str1[1]='h';
	const MyString str3("victory");
	cout<<str1[1]<<endl;	
	cout<<str1<<endl;
	cout<<str3[0]<<endl;
	return 0;
}
