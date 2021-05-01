#include "anytimer.h"

#define JOBSIZE 1024

static volatile sig_atomic_t token;
static struct task* JOB[JOBSIZE];


struct task
{
	int sec;
	at_jobfunc_t func;
	char* arg;
	int status;	
};


void sigalrm_handler(int s)
{
	alarm(1);
	token++;
	
	write(1, ".", 1);
	for(int i = 0; i < JOBSIZE; i++)
		if(JOB[i] && JOB[i]->status)
		{
			JOB[i]->sec--;
			if(!JOB[i]->sec)
			{
				JOB[i]->func(JOB[i]->arg);
				JOB[i]->status = 0;
			}
		}	
}

int at_addjob(int sec, at_jobfunc_t jobp, void* arg)
{
	for(int i = 0; i < JOBSIZE; i++)
	{
		if(JOB[i] == NULL)
		{
			struct task* me;
			me = malloc(sizeof(*me));
			me->sec = sec;
			me->func = jobp;
			me->arg = arg;
			me->status = 1;			
			
			JOB[i] = me;

			return i;
		}
	}


	return -1;
}

void startjob()
{
	signal(SIGALRM, sigalrm_handler);
	alarm(1);
	while(1)
	{	//token--;	
		pause();
	}
}
