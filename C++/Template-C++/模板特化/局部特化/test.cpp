#include<iostream>
using namespace std;

//个数上的偏
//对于bool特殊处理
namespace sp1
{
	template<typename T,typename Alloc>
	class myvector
	{
	public:
		void show(){cout<<"int"<<endl;}				
	};
	
	template<typename Alloc>
	class myvector<bool,Alloc>
	{
	public:
		void show(){cout<<"bool"<<endl;}
	};
}




//范围上的偏
//对于指针类型特殊处理
namespace sp2
{
    template<typename T>
    class test
    {
    public:
        void show(){cout<<"int"<<endl;}
    };
    
    template<typename U>
    class test<U*>
    {
    public:
        void show(){cout<<"int*"<<endl;}
    };
  


}


int main(void)
{
	sp1::myvector<int,int>().show();
	sp1::myvector<bool,int>().show();	 
	sp2::test<int>().show();
    sp2::test<int*>().show();

	return 0;
}
