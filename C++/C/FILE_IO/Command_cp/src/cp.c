#include "io.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		fprintf(stderr,"usage: %s srcfile destfile\n",argv[0]);
	}

	int fdin, fdout;
	fdin = open(argv[1],O_RDONLY);
	if(fdin < 0)
	{
		fprintf(stderr, "open error: %s\n", strerror(errno));
		exit(1);
	}

	fdout = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if(fdout < 0)
	{
		fprintf(stderr, "open error: %s\n", strerror(errno));
	}

	copy(fdin,fdout);

	close(fdin);
	close(fdout);
}
