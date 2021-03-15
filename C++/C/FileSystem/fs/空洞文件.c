#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage : %s <filename>\n", argv[0]);
		exit(-1);
	}

	int fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0600);

	if(fd < 0)
	{
		perror("open error");
		exit(-1);
	}


	lseek(fd, 5LL*1024LL*1024LL*1024LL-1LL, SEEK_SET);
	
	write(fd, "", 1 );

	close(fd);

	return 0;
}
