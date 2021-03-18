#include <stdio.h>
#include <stdlib.h>

int fb(int n)
{

	if(n == 1 || n == 2)
	{
		return 1;
	}

	int val = fb(n-1) + fb(n-2);

	return val;
}


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s [n]\n", argv[0]);
		exit(1);
	}

	if(atoi(argv[1]) < 2)
		exit(-1);

	int n = atoi(argv[1]);

	
	for(int i = 0; i < n; i++)
		printf("%7d", fb(i + 1));
	puts("");	

	return 0;
}
