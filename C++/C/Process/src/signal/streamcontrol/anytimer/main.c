#include <stdio.h>
#include <stdlib.h>
#include "anytimer.h"
#include <signal.h>
#include <errno.h>
#include <string.h>

static void f3(char* p)
{
	printf("f3:%s\n", p);
}

static void f2(char* p)
{
	printf("f2:%s\n", p);
}

static void f1(char* p)
{
	printf("f1:%s\n", p);
}


int main()
{
	int job1, job2, job3;

	puts("Begin");

	job1 = at_addjob(5, f1, "aaa");
	if(job1 < 0)
	{
		fprintf(stderr, "at_addjob:%s\n", strerror(-job1));
		exit(1);
	}

	job2 = at_addjob(2, f2, "bbb");
	if(job2 < 0)
	{
		fprintf(stderr, "at_addjob:%s\n", strerror(-job2));
		exit(1);
	}

	job3 = at_addjob(7, f1, "ccc");
	if(job1 < 0)
	{
		fprintf(stderr, "at_addjob:%s\n", strerror(-job3));
		exit(1);
	}

	startjob();

	puts("End");
	exit(0);
}
