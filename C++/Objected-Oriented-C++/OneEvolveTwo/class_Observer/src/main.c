#include "observer.h"

class MyObserver : public Observer
{
public:
	void update(Object* ths, int value)
	{
		content = value;
	}

	void showView()
	{
		cout<<"that is "<<content<<" in my view."<<endl;
	}

private:
	int content;

};


int main()
{
	//创建目标体
	Object obj;
	
	//创建观察者
	MyObserver os[5];

	//注册观察者
	for(int i = 0; i < 5; i++)
	{
		obj.attach(os+i);
	}
	//目标体变化,观察者同步
	obj.set_val(666);
	
	//遍历观察者视角
	for(int i = 0; i < 5; i++)
	{
		os[i].showView();
	}

	obj.set_val(777);

	//遍历观察者视角
	for(int i = 0; i < 5; i++)
	{
		os[i].showView();
	}
	

	return 0;
}
