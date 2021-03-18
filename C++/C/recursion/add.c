#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int add1(int n)
{
	int sum = 0;
	for(int i = 1; i <= n; i++)
	{
		sum += i;
	}

	return sum;
}



int add2(int n)
{
	if(n == 1)
	{
		return 1;
	}


	return n + add2(n - 1);

}


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage...\n");
		exit(1);
	}
	

	int n = atoi(argv[1]);

	printf("add1:%5d\n", add1(n));
	printf("add2:%5d\n", add2(n));

	return 0;
}
