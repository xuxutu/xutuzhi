#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define THRNUM	20

#define FNAME "out"
#define LINESIZE 1024

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static void *thr_add(void* p)
{
	FILE* pf;
	char linebuf[LINESIZE];

	pf = fopen(FNAME, "r+");
	
	if(!pf)
	{	
		perror("open error: ");
		exit(1);
	}

	pthread_mutex_lock(&mut);

	fgets(linebuf, LINESIZE, pf);
	fseek(pf, 0, SEEK_SET);
	fprintf(pf, "%d\n", atoi(linebuf) + 1);
	fclose(pf);

	pthread_mutex_unlock(&mut);

	pthread_exit(NULL);
}
int main()
{
	int err;
	pthread_t tid[THRNUM];
	
	for(int i = 0; i < THRNUM; i++)
	{
		err = pthread_create(tid + i, NULL, thr_add, NULL);
		if(err)
		{
			fprintf(stderr, "pthread_create error: %s\n", strerror(err));
			exit(1);
		}
	}


	for(int i = 0; i <THRNUM; i++)
	{
		pthread_join(tid[i], NULL);
	}	

	pthread_mutex_destroy(&mut);
	exit(0);
}
