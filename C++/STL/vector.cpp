#include <iostream>

using namespace std;


template<class I>
struct iterator_triats
{
	typedef



};








class alloc{};

template <class T, class Alloc = alloc>
class vector
{
public:	
	typedef T				value_type; 	
	typedef value_type* 	iterator;	//对于这类容器没必要设计一个迭代器类用来访问
	typedef value_type& 	reference;
	typedef size_t 			size_type;

protected:
	iterator start;
	iterator finish;
	iterator end_of_storage;

public:
 //..
	iterator begin(){}
	iterator end(){}

};


void algorithem()
{	
	vector<int> 			vec;
	vector<int>::iterator 	ite = vec.begin(); 




}



int main()
{

	





	return 0;
}
