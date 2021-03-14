#ifndef __SHAPE__
#define __SHAPE__

#include <iostream>
using namespace std;

class Shape
{
public:
	virtual void draw()=0; //纯虚函数，无法实例化对象，子类必须重写。	
};


class Circular : public Shape
{
public:
	void draw()
	{
		cout<<"●"<<endl;
	}

};


class Rectangle	: public Shape
{
public:	
	void draw()
	{
		cout<<"■"<<endl;
	}	
};


class Triangle : public Shape
{
public:
	void draw(){cout<<"▲"<<endl;}
}; 


#endif
