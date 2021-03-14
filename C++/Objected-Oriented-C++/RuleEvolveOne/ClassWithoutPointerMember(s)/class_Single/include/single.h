#ifndef __SINGLE__
#define __SINGLE__
#include <iostream>
using namespace std;
class Single
{
public:
	static Single& getInstance();
	void speak(){cout<<"我独一无二!"<<endl;}
private:
	Single(){cout<<"calling default constructor!"<<endl;}
	Single(const Single& ){}

};




#endif
 
