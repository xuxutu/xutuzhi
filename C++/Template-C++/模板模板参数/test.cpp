#include<iostream>
#include<list>
using namespace std;


template<class T>
class cuz
{


};




namespace sp1
{
	template<typename T,template <typename T> 
						class Container
			>

	class test
	{
	public:
		void show()
		{
			cout<<"shared_ptr<int>c"<<endl;
		}
	private:
		Container<T> c;	
	};

}

namespace sp2
{
	template <class T,class Sequence=deque<T>>

	class stack
	{
	public:
		Sequence c;	
	}



}


int main(void)
{
	//sp1::test<int,list>m1;//此处无法通过，因为容器模板参数不止一个
    sp1::test<int,list> m1;//模板模板参数，list中类型并未确定
	m1.show();
	
	sp2::test<int,list<int>> m2; //非模板模板参数，用的时候指明了

    return 0;
}

