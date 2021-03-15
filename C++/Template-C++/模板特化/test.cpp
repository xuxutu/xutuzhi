#include<iostream>
using namespace std;

template<class Key>
struct Hash
{
	void operator ()(void){cout<<"其他"<<endl;}


};


template<>
struct Hash<char>
{
	void operator ()(void){cout<<"char"<<endl;}
};

int main(void)
{
	
	Hash<char>()();

	return 0;
}
