#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEFT  30000000
#define RIGHT 30000200

int main()
{
	pid_t pid;
	int mark;
	
	for(int i = LEFT; i <= RIGHT; i++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork error:");
		}
		if(pid == 0)
		{
			for(int j = 2; j < i/2; j++)
			{
				mark = 1;
				if(i % j == 0)
				{
					mark = 0;
					break;
				}
			}

			if(mark)
			{
				printf("[%d] is a primer\n", i);
			}
			
			exit(0);
		}

	}

	for(int i = LEFT; i <= RIGHT; i++)
		wait(0);
}

