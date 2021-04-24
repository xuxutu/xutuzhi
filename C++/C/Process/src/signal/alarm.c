#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
	alarm(10);
	alarm(5);
	alarm(5);

	while(1) //忙等
	pause();  //解决忙等
	puts("haha");

	return 0;
}
