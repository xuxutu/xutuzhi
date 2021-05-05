#include "mytbf.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

struct mytbf_st
{
	int cps;
	int burst;
	int token;
	int pos;
};
static struct mytbf_st* job[MYTBF_MAX];
static int inited = 0;
typedef void (*sighandler_t)(int);

//static sighandler_t save;
static struct sigaction alrm_sa_save;


static void alrm_action(int s, siginfo_t* info, void* unused)
{
//	alarm(1);

	if(info->si_code != SI_KERNEL)
		return;

	for(int i = 0; i < MYTBF_MAX; i++)
	{
		if(job[i] != NULL)
		{
			job[i]->token += job[i]->cps;
			if(job[i]->token > job[i]->burst)
				job[i]->token = job[i]->burst;
		}
	}
}		



static void module_unload()
{
	//给一个函数定义新的新为的时候返回旧的行为
	//signal(SIGALRM, save);
	//alarm(0);

	struct itimerval itv;
	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = 0;
	
	sigaction(SIGALRM, &alrm_sa_save, NULL);

	setitimer(ITIMER_REAL, &itv, NULL);

	for(int i = 0; i < MYTBF_MAX; i++)
		free(job[i]);	
}

static void module_load()
{
	struct itimerval itv;

	struct sigaction sa;
	sa.sa_sigaction = alrm_action;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;

	sigaction(SIGALRM, &sa, &alrm_sa_save);
	
	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;
	

	setitimer(ITIMER_REAL, &itv, NULL);
	//save = signal(SIGALRM, alrm_handler);
	//alarm(1);

	atexit(module_unload);

}


static int get_free_pos()
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

	if(!inited)
	{
		//signal(SIGALRM,)
		//alarm(1);
		module_load();
		inited = 1;
	}

	pos = get_free_pos();
	if(pos < 0)
	{
		return NULL;
	}


	me = malloc(sizeof(*me));
	if(me == NULL)
		return NULL;

	me->token = 0;
	me->cps = cps;
	me->burst = burst;
	job[pos] = me;
	
	me->pos = pos;

	return me;
}

int mytbf_fetchtoken(mytbf_t* tbf, int size)
{
	struct mytbf_st* me = tbf;
	int n;

	if(size <= 0)
		return -EINVAL;

	while(me->token <= 0)
		pause();

	n = me->token < size ? me->token : size;

	me->token -= n;

	return n;
}

int mytbf_returntoken(mytbf_t* ptr, int size)
{
	struct mytbf_st* me = ptr;

	if(size <= 0)
		return -EINVAL;
	me->token += size;
	
	if(me->token > me->burst)
		me->token = me->burst;	

	return size;	
}	

int mytbf_destroy(mytbf_t* ptr)
{
	struct mytbf_st* me = ptr;
	job[me->pos] = NULL;
	free(ptr);
	
	return 0;
}

