#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b)\
		({\
			typeof(a) _a = a; \
			typeof(b) _b = b; \
			(_a) > (_b) ? _a : (_b); \	
			_b = 200; \
			_a = 190; \	
			int f;\
			f = 1000;\
		})

//一个宏的返回值是最后一行的变量
//宏参数是表达式，应该带括号
//宏传递不带类型，是内敛函数无法替代的

#define MACRO(data, i)\
	printf("%d\n", data##i)	

int main()
{
	int a, b = 0;

	int c = MAX(a, b);

	printf("%d\n", c);
//------------------
	int data1 = 11000;
	int data2 = 12000;

	MACRO(data, 1);
	MACRO(data, 2);

	return 0;
}
