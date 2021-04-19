#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	puts("Begin!");

	pid = fork();

	if(pid < 0)
	{
		perror("fork error:");
		exit(-1);
	}
	if(pid == 0)
	{
		execl("/bin/date", "date", "+%s", NULL);
		perror("execl error:");
		exit(1);
	}
	
	wait(NULL);

	puts("End");

	exit(0);
}
