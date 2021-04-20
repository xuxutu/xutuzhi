#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>



int main(int argc, char* argv[])
{
	pid_t pid;

	pid = fork();

	if(pid < 0)
		perror("fork");
	if(pid == 0)
	{
		setuid(atoi(argv[1]));
		execvp(argv[2], argv + 2);
		perror("execvp error");
		exit(1);
	}

	wait(0);

	return 0;
}
