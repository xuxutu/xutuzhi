#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("usage:%s fifo\n", argv[0]);
		exit(1);
	}

	printf("open fifo read...\n");

	int fd = open(argv[1], O_RDONLY);
	if(fd < 0)
	{
		perror("open error");
	}
	else
	{
		printf("open file succes: %d\n", fd);
	}

	char buf[512];

	while(read(fd, buf, sizeof(buf)) < 0)
	{
		perror("read error");	
	}

	puts(buf);

	exit(0);
}
