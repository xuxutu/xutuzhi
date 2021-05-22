#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define THRUM 4


static pthread_mutex_t mut[THRUM];


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
	//pthread_mutex_lock(mut + n);//一初始化就上锁

	while(1)
	{
		pthread_mutex_lock(mut + n);
		write(1, &c, 1);
		pthread_mutex_unlock(mut + next(n));
	}
	pthread_exit(0);
}

int main()
{
	int i, err;
	pthread_t tid[THRUM];

	for(i = 0; i < THRUM; i++)
	{
		pthread_mutex_init(mut + i, NULL);
		pthread_mutex_lock(mut + i);//一初始化就上锁
		if(err = pthread_create(tid + i, NULL, thr_func, (void*)i))
		{
			fprintf(stderr, "pthread create error : %s\n", strerror(err));
			exit(1);
		}
		else	
		{
			printf("pthread_create succeed!\n");
		}	
	}

//	alarm(1);

	//打开一个锁
	pthread_mutex_unlock(mut + 0);
	
	for(i = 0; i < THRUM; i++)
		pthread_join(tid[i], NULL);

	return 0;
}
