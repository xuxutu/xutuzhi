#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	int fd[2];
	if(pipe(fd) < 0)
	{
		perror("pipe error");
		exit(1);
	}

	pid_t pid;

	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid > 0)
	{
		//parent process
		sleep(3); //Ensure child process has become imcomplete pipe.		
		close(fd[1]);

		while(1)
		{
			char c;
			if((read(fd[0], &c, 1)) == 0)
			{
				printf("\nwrite-end of pipe closed\n");
				break;
			}
			else
			{
				printf("%c", c);
			}	
		}

		close(fd[0]);
		wait(0);
	}
	else
	{
		//child process
		close(fd[0]);//close read endian.
		//write data to pipe
		char* s = "1234";
		write(fd[1], s, sizeof(s));
		//close write endian.
		close(fd[1]);
	}



	return 0;
}
