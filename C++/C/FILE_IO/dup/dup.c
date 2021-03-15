#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FNAME "/tmp/out"

int main()
{
	int fd = open(FNAME, O_WRONLY|O_CREAT|O_TRUNC, 0600);
	if(fd < 0)
	{
		perror("open error :");
		exit(-1);
	}


	//close(1);
	//dup(fd); //把fd复制在最小的未使用的位置，也就是上行所关闭的1的位置

	dup2(fd, 1); //close 1, 复制fd到1所在位置,如果fd == 1，什么也不做，返回fd
	
	if(fd != 1)
	{
		close(fd);//关闭原来的fd，打开计数器直到为0时才关闭
	}
	


/*******************************************************/
	puts("hello!");

	exit(-1);
}
