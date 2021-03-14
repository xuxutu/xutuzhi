#include <iostream>
#include <vector>
using namespace std;


template<class InputIterator, class Predicate>

/*typename iterator_triats<InputIterator>::difference_type*/int
Count_if(InputIterator first, InputIterator last, Predicate pred)
{
	//算法提问
	int/*typename iterator_triats<InputIterator>::difference_type*/ n = 0;
	for(; first != last; ++first)
	{
		if(pred(*first))
			++n;
	}
		
	return n;
}

template<class Arg, class Result>
struct Unary_function
{	
	typedef Arg argument_type;
	typedef Result result_type;
};


template<class Arg1, class Arg2, class Result>
struct Binary_function
{
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};


template<class Predicate>
class Unary_negate : public Unary_function<typename Predicate::argument_type, bool>
{
protected:
	Predicate pred;
public:
	explicit Unary_negate(const Predicate& x) : pred(x){}

	bool operator()(const typename Predicate::argument_type& x)const
	{

		return !pred(x);
	}	

};

template<class Predicate>
Unary_negate<Predicate> Not1(const Predicate& pred)
{
	return Unary_negate<Predicate>(pred);
}

template<class Operation>
class Binder2nd : public Unary_function<
										typename Operation::first_argument_type,
										typename Operation::result_type			
																		>
{
protected:
	Operation op;
	
	const typename Operation::second_argument_type value;
public:
	Binder2nd(const Operation& x,
			  const typename Operation::second_argument_type& y) : op(x), value(y)
	{
		//cout<<value<<endl;
	}

typename Operation::result_type
operator()(const typename Operation::first_argument_type& x)const
{
	return op(x, value);
}

};

template<class Operation, class T>
inline Binder2nd<Operation> Bind2nd(const Operation& op, const T& x)
{
	typedef typename Operation::second_argument_type arg2_type;


//	Binder2nd<Operation> y(op, arg2_type(x));	
	
	return Binder2nd<Operation>(op, arg2_type(x));
	//return y;
}

template<typename T>
struct Less : public Binary_function<T, T, bool>
{
	bool operator ()(const T& x, const T& y)const
	{
		//cout<<"["<<x<<"]"<<"["<<y<<"]"<<endl;
		return x < y;
	}
};




int main(void)
{
	vector<int> v1{1,2,3,4,5,6};
	
	cout<<Count_if(v1.begin(), v1.end(), Not1(Bind2nd(Less<int>(), 4)))<<endl;



	return 0;
}
