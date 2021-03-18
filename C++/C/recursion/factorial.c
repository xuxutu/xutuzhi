#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fac(int n)
{
	if(n == 1)
		return 1;

	return n * fac(n-1);
}
int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage...\n");
		exit(-1);
	}

	int n = atoi(argv[1]);

	printf("%5d\n", fac(n));

	return 0;
}

