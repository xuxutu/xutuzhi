#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct
{
	int res;
	int is_wait; //condition give by user for judgment
	pthread_cond_t cond; //condition variable
	pthread_mutex_t mutex; 
}Result;


//Runing thread function that calculate the result and put result in the Result sructure.
void* set_fn(void* arg)
{




	return (void*)0;
}


//Runing thread function that get the result calculate by set_fn.
void* get_fn(void* arg)
{



	return (void*)0;
}

int main()
{




	return 0;
}
