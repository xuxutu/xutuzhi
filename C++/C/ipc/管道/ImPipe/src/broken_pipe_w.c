#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
void sig_handler(int signo)
{
	if(signo == SIGPIPE)
	{
		printf("SIGPIPE occured\n");
	}
}


int main()
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
		sleep(3);
		close(fd[0]);		
 		
		//sign signal function
		if(signal(SIGPIPE, sig_handler) == SIG_ERR)
		{
			perror("signal error");
			exit(1);
		}

		char* s = "1234";
		if(write(fd[1], s, sizeof(s)) != sizeof(s))
		{
			fprintf(stderr, "%s, %s\n", strerror(errno), ((errno == EPIPE) ? "EPIPE" : "unknow"));
		}

		close(fd[1]);
		wait(0);	
	}
	else
	{
		//child process
		close(fd[0]);
		close(fd[1]);
	}


	return 0;
}
