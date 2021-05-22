#include "mytbf.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

struct mytbf_st
{
	int cps;
	int burst;
	int token;
	int pos;
	pthread_mutex_t mut;
};
static struct mytbf_st* job[MYTBF_MAX];
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_t tid_alrm;

static pthread_once_t init_once = PTHREAD_ONCE_INIT;
//static int inited = 0;

//typedef void (*sighandler_t)(int);

//static sighandler_t save;



static void *thr_alrm(void* s)
{
	//alarm(1);

	while(1)
	{
		pthread_mutex_lock(&mut_job);
		for(int i = 0; i < MYTBF_MAX; i++)
		{
			if(job[i] != NULL)
			{
				pthread_mutex_lock(&job[i]->mut);
				job[i]->token += job[i]->cps;
				if(job[i]->token > job[i]->burst)
					job[i]->token = job[i]->burst;
				pthread_mutex_unlock(&job[i]->mut);
			}
		}
		pthread_mutex_unlock(&mut_job);
		sleep(1);
	}
}		



static void module_unload()
{
	//给一个函数定义新的新为的时候返回旧的行为
	//signal(SIGALRM, save);
	//alarm(0);
	pthread_cancel(tid_alrm);
	pthread_join(tid_alrm, NULL);

	for(int i = 0; i < MYTBF_MAX; i++)
	{
		if(job[i] != NULL)
		{
			mytbf_destroy(job[i]);
		}
	}
	pthread_mutex_destroy(&mut_job);
}

static void module_load()
{
	//save = signal(SIGALRM, alrm_handler);
	//alarm(1);
	int err;
	err = pthread_create(&tid_alrm, NULL, thr_alrm, NULL);
	if(err)
	{
		fprintf(stderr, "pthread_create error : %s\n", strerror(err));
		exit(1);
	}	

	atexit(module_unload);

}


static int get_free_pos_unlocked()
{
	for(int i = 0; i < MYTBF_MAX; i++)
	{
		if(job[i] == NULL)
			return i;
	}

	return -1;
}

mytbf_t* mytbf_init(int cps, int burst)
{
	struct mytbf_st* me;
	int pos;

/*
	if(!inited)
	{
		//signal(SIGALRM,)
		//alarm(1);
		module_load();
		inited = 1;
	}
*/

	pthread_once(&init_once, module_load);

	me = malloc(sizeof(*me));
	if(me == NULL)
		return NULL;

	me->token = 0;
	me->cps = cps;
	me->burst = burst;
	pthread_mutex_init(&me->mut, NULL);

	pthread_mutex_lock(&mut_job);
	pos = get_free_pos_unlocked();
	if(pos < 0)
	{
		pthread_mutex_unlock(&mut_job);
		free(me);
		return NULL;
	}

	job[pos] = me;
	me->pos = pos;

	pthread_mutex_unlock(&mut_job);

	return me;
}

int mytbf_fetchtoken(mytbf_t* tbf, int size)
{
	struct mytbf_st* me = tbf;
	int n;

	if(size <= 0)
		return -EINVAL;

	pthread_mutex_lock(&me->mut);
	while(me->token <= 0)
	{
		pthread_mutex_unlock(&me->mut);
		sched_yield();
		pthread_mutex_lock(&me->mut);
	}

	n = me->token < size ? me->token : size;

	me->token -= n;
	pthread_mutex_unlock(&me->mut);

	return n;
}

int mytbf_returntoken(mytbf_t* ptr, int size)
{
	struct mytbf_st* me = ptr;

	if(size <= 0)
		return -EINVAL;

	pthread_mutex_lock(&me->mut);
	me->token += size;

	if(me->token > me->burst)
		me->token = me->burst;	
	pthread_mutex_unlock(&me->mut);

	return size;	
}	

int mytbf_destroy(mytbf_t* ptr)
{
	struct mytbf_st* me = ptr;\

	pthread_mutex_lock(&mut_job);
	job[me->pos] = NULL;
	pthread_mutex_unlock(&mut_job);

	pthread_mutex_destroy(&me->mut);
	free(ptr);
	
	return 0;
}

