#include<iostream>
using namespace std;

template<class T>
class shared_ptr
{
public:
	shared_ptr(T* p) : px(p){}

	T& operator*()const
	{
		return *px;
	}

	T* operator->()const
	{
		return px;
	}

private:
	T* px;
	long* pn;

};

struct Foo
{
	void speak(){}
};

int main(void)
{
	shared_ptr<Foo> sp(new Foo());
    Foo f(*sp);

    sp->speak();//编译器会自动加->

	return 0;
}	
