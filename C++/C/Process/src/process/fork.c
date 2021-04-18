#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	
	printf("[%d]:Begin!\n", getpid());
	
	fflush(NULL);	

	pid = fork();
	if(pid < 0)
	{
		perror("fork error:");
		exit(-1);
	}
	if(pid == 0)//child
	{
		printf("[%d]:Child is working\n", getpid());
	}
	if(pid > 0)
	{
		printf("[%d]:Parent is working\n", getpid());
	}

	printf("[%d]:End!\n", getpid());

		
	return 0;
}
