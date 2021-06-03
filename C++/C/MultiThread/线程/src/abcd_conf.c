#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define THRUM 4

static int num = 0;
static pthread_mutex_t mut;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; //初始化条件变量

static int next(int n)
{
	if(n + 1 == THRUM)
		return  0;

	return ++n;
}

static void *thr_func(void *ptr)
{
	int n = (int)ptr;
	int c = 'a' + n;

	while(1)
	{
		pthread_mutex_lock(&mut);	
	
		while(n != num)
			pthread_cond_wait(&cond, &mut);

		write(1, &c, 1);

		num = next(n);		

		sleep(1);	
		pthread_cond_broadcast(&cond);//只能用广播
		pthread_mutex_unlock(&mut);
		
	}
	pthread_exit(0);
}

int main()
{
	int i, err;
	pthread_t tid[THRUM];

	for(i = 0; i < THRUM; i++)
	{
		//开启四个线程
		if(err = pthread_create(tid + i, NULL, thr_func, (void*)i))
		{
			fprintf(stderr, "pthread create error : %s\n", strerror(err));
			exit(1);
		}
	}


	for(i = 0; i < THRUM; i++)
		pthread_join(tid[i], NULL);

	return 0;
}
