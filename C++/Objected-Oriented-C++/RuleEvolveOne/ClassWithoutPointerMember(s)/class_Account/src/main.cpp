#include "account.h"
#include <iostream>
using namespace std;

int main(void)
{
	cout<<Account::m_rate<<endl;
	Account at;
	at.set_rate(11.2);
	cout<<at.m_rate<<endl;
	cout<<Account::m_rate<<endl;
	return 0;
}
