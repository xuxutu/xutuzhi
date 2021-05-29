#include <stdio.h>
#include <stdlib.h>


void test1()
{
	puts("moudle end!");
}

void test()
{
	atexit(test1);
	printf("start!\n");

	exit(0);
}

int main()
{
	test();

	puts("new moudle start!");

	return 0;
}
