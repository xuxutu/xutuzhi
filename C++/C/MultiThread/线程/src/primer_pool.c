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
#define THRNUM	4

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_num = PTHREAD_COND_INITIALIZER;

static void* thr_primer(void* p)
{
	int i, j, mark;
	
	while(1)
	{
		pthread_mutex_lock(&mut_num);
		while(num == 0)	
		{
			pthread_cond_wait(&cond_num, &mut_num);	
			/*
			pthread_mutex_unlock(&mut_num);
			sched_yield();
			pthread_mutex_lock(&mut_num);
			*/
		}

		if(num == -1)
		{
			pthread_mutex_unlock(&mut_num);
			break;//临界区内的跳转目的地在临界区外，要解锁再跳出
		}

		i = num;
		num = 0;

		pthread_cond_broadcast(&cond_num);

		pthread_mutex_unlock(&mut_num);


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
			printf("[%d]%d is a primer\n", (int)p, i);
		}
	}
	pthread_exit(NULL);
}



int main()
{
	pthread_t tid[THRNUM];
	int err, i;


	for(i = 0; i < THRNUM; i++)
	{
		err = pthread_create(tid + i, NULL, thr_primer, (void*)i);
		if(err)
		{
			fprintf(stderr, "ptherad_create error : %s\n", strerror(err));
			exit(1);
		}
	}

	for(i = LEFT; i <= RIGHT; i++)
	{
		pthread_mutex_lock(&mut_num);
		while(num != 0)
		{
			pthread_cond_wait(&cond_num, &mut_num);
			/*
			pthread_mutex_unlock(&mut_num);
			//sleep 会造成不必要的调度颠簸
			sched_yield();//出让调度器给别的线程，非常短的sleep
			pthread_mutex_lock(&mut_num);
			*/
		}	
		num = i;
		pthread_cond_signal(&cond_num);
		pthread_mutex_unlock(&mut_num);
	}

	
	pthread_mutex_lock(&mut_num);
	while(num != 0)
	{
		pthread_mutex_unlock(&mut_num);
		sched_yield();
		pthread_mutex_lock(&mut_num);
	}
	
	num = -1;
	pthread_cond_broadcast(&cond_num);
	pthread_mutex_unlock(&mut_num);
	
	for(i = 0; i < THRNUM; i++)
	{
		pthread_join(tid[i], NULL);
	}

	pthread_mutex_destroy(&mut_num);
	pthread_cond_destroy(&cond_num);

	exit(0);
}

