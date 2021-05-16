#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>


#define LEFT  30000000
#define RIGHT 30000200
#define THRNUM	(RIGHT - LEFT + 1)


static void* thr_primer(void* p)
{
	int i, j, mark;
	
	i = *(int*)p;
	
	for(j = 2; j < i/2; j++)
	{
		mark = 1;
		if(i % j == 0)
		{
			mark = 0;
			break;
		}
	}

	if(mark)
	{
		printf("[%d] is a primer\n", i);
	}

	pthread_exit(NULL);
}



int main()
{
	pthread_t tid[THRNUM];
	int err;


	for(int i = LEFT; i < RIGHT; i++)
	{
		err = pthread_create(tid + (i - LEFT), NULL, thr_primer, &i);
		if(err)
		{
			fprintf(stderr, "ptherad_create error : %s\n", strerror(err));
			exit(1);
		}
	}
	
	for(int i = LEFT; i <= RIGHT; i++)
	{
		pthread_join(tid[i-LEFT], NULL);
	}


	exit(0);
}

