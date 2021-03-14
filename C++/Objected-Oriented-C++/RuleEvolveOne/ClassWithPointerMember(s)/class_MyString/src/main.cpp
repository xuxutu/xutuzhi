#include "mystring.h"

int main(void)
{
	MyString str1("hello");
	/*cout<<str1.get_c_str()<<endl;
	MyString str2(str1);
	cout<<str2.get_c_str()<<endl;*/
	MyString str3=str1;
	str3=str1;
	cout<<str3.get_c_str()<<endl;

	cout<<MyString("axiba")<<endl;

	cout<<str3[3]<<endl;
	str3[1]='x';
	cout<<str3<<endl;			


		
	return 0;
}
