#include <stdio.h>
#include <stdlib.h>

/*Determine whether arguments can be added without overflow */
int add_ok(int x, int y)
{
	int sum = x + y;
	int neg_over = x < 0 && y < 0 && sum >= 0;
	int pos_over = x >= 0 && y >= 0 && sum < 0;

	return !neg_over && !pos_over; 
}

int main(int argc, char* argv[])
{
	int sum = 0;
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	
	if(add_ok(x,y))
	{
		puts("without overflow！");
	}
	else
	{
		puts("overflow！");
	}

	return 0;
}
