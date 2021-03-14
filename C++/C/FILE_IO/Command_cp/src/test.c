#include "io.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>


int main()
{
	if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
	{
		perror("dup2 error");
	}

	set_fl(STDOUT_FILENO, O_NONBLOCK);
	
	puts("城南小陌又逢春，只见梅花不见人");

	char buf[512];
	read(STDOUT_FILENO, buf, 512);	
	write(STDERR_FILENO, buf, strlen(buf));	

	puts("\n");
	return 0;
}
