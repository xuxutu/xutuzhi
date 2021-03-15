#include<iostream>
#include<vector>
using namespace std;


class shape
{
public:
	virtual void show()=0;


};

class tyuan : public shape
{
public:
	virtual void show(){cout<<"我是椭圆!"<<endl;}

};


class yuan : public tyuan
{
	virtual void show(){cout<<"我是圆!"<<endl;}


};

//c++编译器看到函数会进行考量：动态绑定，还是静态绑定。满足三个条件会动态绑定：1.必须是通过指针来调用 2.upcast(向上转型) 3.调用的是虚函数。
//只要符合这三个条件，编译器就会编译成这种样子：(*(p->vptr)[n])(p);虚机制


int main(void)
{
	//多态
	/*vector<shape*> paint;
    paint.push_back(new tyuan());
	paint.push_back(new yuan());
	paint[0]->show();
	paint[1]->show();	*/	

	//((void(*)())(*(int*)*(int *)animal))();
	shape* sp=new tyuan;
	sp->show();
	//((void(*)())(*(int *)(*(int*)sp)))();

	((void(*)())(*(int*)(*(int*)sp)))();

	return 0;
}
