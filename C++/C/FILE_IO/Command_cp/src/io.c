#include "io.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFER_LEN 1024

void copy(int fdin, int fdout)
{

	char buffer[BUFFER_LEN];

	ssize_t size;
	while((size = read(fdin, buffer, BUFFER_LEN)) > 0)
	{	
		
		if(write(fdout, buffer, size) != size)
		{
			fprintf(stderr,"write error: %s\n",strerror(errno));
			exit(1);
		}
	}

	if(size < 0)
	{
		fprintf(stderr, "read error: %s\n",strerror(errno));
		exit(1);
	}
}


void set_fl(int fd, int flag)
{
	//获得原来的文件状态标志
	int val = fcntl(fd, F_GETFL);
	//增加新的文件状态标志
	val |= flag;
	//重新设置文件状态标志
	if(fcntl(fd, F_SETFL, val) < 0)
	{
		perror("fcntl error");
	}
}


void clr_fl(int fd, int flag)
{
	//获得原来的文件状态标志
	int val = fcntl(fd, F_GETFL);
	//清楚指定的的文件状态标志
	val &= ~flag;

	
	//重新设置文件状态标志
	if(fcntl(fd, F_SETFL, val) < 0)
	{
		perror("fcntl error");	
	}
	
}
