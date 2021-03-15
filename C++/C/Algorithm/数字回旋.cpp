#include <iostream>
using namespace std;

void print(int* s, int n)
{
	for(int i = 0; i < n*n; i++)
	{
		printf("%5d", s[i]);
		if(((i + 1) % n) == 0)
			cout<<endl;
	}
}

/*
	1  2  3  4
	12 13 14 5
	11 16 15 6
    10 9  8  7			 
*/

/*

  1  2  3  4  5  6  7
 24  0  0  0  0  0  8
 23  0  0  0  0  0  9
 22  0  0  0  0  0 10
 21  0  0  0  0  0 11
 20  0  0  0  0  0 12
 19 18 17 16 15 14 13

*/

void _snake(int* s, int n, int count, int start, int N)
{
	if(n <= 0)
	{
		return;
	}

	int i = 0;

	while(i < n)
	{
		s[count++] = start++;
		i++;
	}

	--count;//归位
	--start;
	i = 0;	

	while(i < n)
	{
		s[count] = start++;
		count += N;
		i++;
	}

	i = 0;
	count -= N;
	--start;

	while(i < n)
	{
		s[count--] = start++;
		i++;	
	}

	i = 0;
	++count;
	--start;
	
	while(i < n-1)
	{
		s[count] = start++;
		count -= N;
		i++;
	}

	count += N;
	++count;
	
	_snake(s, n-2, count, start, N);

}

void snake(int *s, int n)
{
	_snake(s, n, 0, 1, n);
}

int main(int argc, char* argv[])
{

	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s [number]\n", argv[0]);

		exit(-1);
	}

	const int N = atoi(argv[1]);

	int space[N][N] = {0};

	int (*p)[N] = space;

	snake(*space, N);
	print(*space, N);

	return 0;
}
