#include<iostream>
using namespace std;


//variadic templates(数量不定的模板参数)



void print()
{

	cout<<"print end!"<<endl;
}

template<typename T,typename... Types>
void print(T firstArg,Types... args)
{
	cout<<firstArg<<endl;
	print(args...);
    //cout<<sizeof...(args)<<endl;
}




int main()
{

	print(7.5,"hello",42);


	return 0;
}

