#ifndef __DOCUMENT__
#define __DOCUMENT__
#include <iostream>
using namespace std;

class Document
{
public:	
	//非虚函数，我不希望子类重写
	void OnFileOpen()
	{
		cout<<"第一步"<<endl;
		cout<<"第二步"<<endl;
		cout<<"第三步"<<endl;
		serialize();
		cout<<"第五步"<<endl;
	}

	virtual void serialize(){}//子类去重写

};



#endif
