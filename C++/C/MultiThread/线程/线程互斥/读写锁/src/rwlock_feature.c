#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//定义读写锁
pthread_rwlock_t rwlock;

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("usage:%s [r|w] [r|w]\n", argv[0]);
	}

	//读写锁初始化

	pthread_rwlock_init(&rwlock, NULL);

	if(!strcmp("r", argv[1]))
	{
		//加读锁
		if(pthread_rwlock_rdlock(&rwlock) != 0)
		{
			puts("first read lock failure");
		}
		else
		{
			puts("first read lock success");
		}
	}
	else if(!strcmp("w",argv[1]))
	{
		//加写锁
		if(pthread_rwlock_wrlock(&rwlock) != 0)
		{
			puts("first write lock failure");
		}
		else
		{
			puts("first write lock success");
		}
	}

	//读写锁初始化
	if(!strcmp("r", argv[2]))
	{
		//加读锁
		if(pthread_rwlock_rdlock(&rwlock) != 0)
		{
			puts("second read lock failure");
		}
		else
		{
			puts("second read lock success");
		}
	}
	else if(!strcmp("w",argv[2]))
	{
		//加写锁
		if(pthread_rwlock_wrlock(&rwlock) != 0)
		{
			puts("second write lock failure");
		}
		else
		{
			puts("second write lock success");
		}
	
	}
	
	//释放锁
	pthread_rwlock_unlock(&rwlock);
	pthread_rwlock_unlock(&rwlock);	

	return 0;
}
