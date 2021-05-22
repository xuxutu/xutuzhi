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


struct thr_arg_st
{
	int n;
};

static void* thr_primer(void* p)
{
	int j, mark;
	
	int i = ((struct thr_arg_st*)p)->n;
	
	for(j = 2; j < i/2; j++)
	{
		mark = 1;
		if((i % j) == 0)
		{
			mark = 0;
			break;
		}
	}

	if(mark)
	{
		printf("[%d] is a primer\n", i);
	}

	free(p);

	pthread_exit(0);
}



int main()
{
	pthread_t tid[THRNUM];
	struct thr_arg_st *p;
	int err;

	void* ptr; 


	for(int i = LEFT; i < RIGHT; i++)
	{
	//	printf("i = %d\n", i);
		p = malloc(sizeof(struct thr_arg_st));
		if(!p)
			exit(1);
		p->n = i;

		err = pthread_create(tid + (i - LEFT), NULL, thr_primer, p);
		if(err)
		{
			fprintf(stderr, "ptherad_create error : %s\n", strerror(err));
			exit(1);
		}
	}
	
	for(int i = LEFT; i <= RIGHT; i++)
	{
		//pthread_join(tid[i-LEFT], &ptr);
		//free(ptr);
		pthread_join(tid[i-LEFT], NULL);
	}


	exit(0);
}

