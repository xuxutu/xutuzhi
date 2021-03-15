#include<iostream>
using namespace std;

//鱼类
class Fish{};


//鸟类
class Bird{};

//鸟
class fish : public Fish{};

//鱼
class bird : public Bird{};

template<class T1,class T2>
struct Pair
{
	Pair():first(T1()),second(T2()){}

	Pair(const T1& a,const T2& b):first(a),second(b){}

	template<class U1,class U2>
	Pair(const Pair<U1,U2>&p):first(p.first),second(p.second){}

	T1 first;
	T2 second;
};

int main(void)
{
	Pair<fish,bird>p;
	Pair<Fish,Bird>p2(p);
	


	return 0;
}
