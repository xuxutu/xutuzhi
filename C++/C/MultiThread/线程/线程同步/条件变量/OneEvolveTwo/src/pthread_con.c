#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
	int res;
	int counter;			//Count the number of threads that get results
	pthread_cond_t cond;	//condition variable 
	pthread_mutex_t mutex;  //mutex
}Result;


//thread runing function
//calculate result and put it in Result structure
void* set_fn(void* arg)
{
	Result* r = (Result*)arg;
	
	int sum = 0;
	for(int i = 1; i <= 100; i++)
	{
		sum += i;
	}

	r->res = sum;
	//Protect shared resources
	pthread_mutex_lock(&r->mutex);

	//Determine whether the number of threads to get the result reaches specifiled numer. 
	while(r->counter < 2)
	{
		pthread_mutex_unlock(&r->mutex);
		usleep(100);
		pthread_mutex_lock(&r->mutex);
	}

	//wake waiting thread
	pthread_cond_broadcast(&r->cond);
	

	pthread_mutex_unlock(&r->mutex);

	return (void*)0;
}

//thread runing function
//get result in Result structure
void* get_fn(void* arg)
{
	Result* r = (Result*)arg;

	//Protect shared resources
	pthread_mutex_lock(&r->mutex);
	r->counter++;
	//waiting	
	pthread_cond_wait(&r->cond, &r->mutex);
	/*1.unlock(&mutex)
	 *2.lock(&mutex)	
	 *3.set thread self into queque
	 *4.unlock(&mutex);
	 *5.block and wait other thread wake self 
	 *6.lock(&mutex)
	 *7.delete thread self from queue		
	 */	
	
	pthread_mutex_unlock(&r->mutex);

	//get result
	int res = r->res;	
	
	printf("0x%lx get sum is %d\n", pthread_self(), res);
	return (void*)0;
}

//Here are ensure that two functions don't access the data at the same time.So not need to add lock for r->res


int main(void)
{
	int err;
	pthread_t cal, get1,get2;	
	Result r;
	r.counter = 0;

	pthread_cond_init(&r.cond, NULL);
	pthread_mutex_init(&r.mutex, NULL);

	if((err = pthread_create(&get1, NULL, get_fn, (void*)&r)) != 0)	
	{
		perror("pthread create error");
	}

	if((err = pthread_create(&get2, NULL, get_fn, (void*)&r)) != 0)
	{
		perror("pthread create error");	
	}


	if((err = pthread_create(&cal, NULL, set_fn, (void*)&r)) != 0)
	{
		perror("pthread create error");	
	}

	//main thread wait for other threads
	pthread_join(cal, NULL);	
	pthread_join(get1, NULL);
	pthread_join(get2, NULL);
	
	pthread_cond_destroy(&r.cond);
	pthread_mutex_destroy(&r.mutex);

	
	return 0;
}
