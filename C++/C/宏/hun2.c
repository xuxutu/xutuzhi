#include <stdio.h>
#include <stdlib.h>

#define var(format, arg...) fprintf (stderr, format, ##arg)


//#define var(str) print(str);

//定义函数时防止宏扩展
void (var)(char* str)
{
	puts(str);
}

int main()
{

	var("%s,%s", "hello\n", "hi\n");
	var("hello");



	return 0;
}
