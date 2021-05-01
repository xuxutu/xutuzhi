#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "mytbf.h"
#include <errno.h>
#include <string.h>

#define CPS 1024
#define BUFSIZE 1024
#define BURST 1024


int main(int argc, char** argv)
{

	int sfd, dfd = 1;
	mytbf_t* tbf;
	int size;


	if(argc < 2)
	{
		fprintf(stderr, "usage: %s desfile\n", argv[0]);
		exit(1);
	}	

	tbf = mytbf_init(CPS, BURST);
	if(tbf == NULL)
	{	fprintf(stderr, "mytbf_init failed:\n");
		exit(1);
	}

	sfd = open(argv[1], O_RDONLY);
	if(sfd < 0)
	{
		perror("open error");
		exit(1);
	}

	int len;
	int ret;
	int pos;

	char buf[BUFSIZE];

	while(1)
	{
		size = mytbf_fetchtoken(tbf, BUFSIZE);
		if(size < 0)
		{
			fprintf(stderr, "mytbf_fetchtoken failed:%s\n", strerror(-size));
			exit(1);
		}

		while((len = read(sfd, buf, size)) < 0)
		{
			if(errno == EINTR)
				continue;
		
			perror("read error");
			break;	
		}

		if(len <= 0)
		{
			break;
		}
	
		if(size - len > 0)
			mytbf_returntoken(tbf, size - len);			

		pos = 0;
		while(len > 0)
		{
			ret = write(dfd, buf + pos, len);
		
			if(ret < 0)
			{
				if(errno == EINTR)
					continue;
				perror("write error");
				exit(1);
			}
			
			pos += ret;
			len -= ret;	
		}
	}

	close(sfd);
	mytbf_destroy(tbf);

	return 0;
}
