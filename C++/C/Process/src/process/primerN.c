#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEFT  30000000
#define RIGHT 30000200
#define N 3

int main()
{
	pid_t pid;
	int mark;

	for(int n = 0; n < N; n++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork error:");
			for(int i = 0; i < n; i++)
				wait(0);
			exit(0);
		}
		if(pid == 0)
		{
			for(int i = LEFT + n; i <= RIGHT; i += N)
			{
				mark = 1;
				for(int j = 2; j < i/2; j++)
				{
					if(i % j == 0)
					{
						mark = 0;
						break;
					}
				}
			
				//printf("[i]%d\n", i);
	
				if(mark)
				{
					printf("[%d]%d is a primer\n", n, i);
				}
			}

			exit(0);
		}
	}

//	puts("run end!");
	for(int i = 0; i < N; i++)
		wait(NULL);
}

