#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 1024	

int main(int argc, char** argv)
{

	if(argc < 3)
	{
		fprintf(stderr, "usage: %s srcfile desfile\n", argv[0]);
		exit(1);
	}	

	int sfd, dfd;


	sfd = open(argv[1], O_RDONLY);
	if(sfd < 0)
	{
		perror("open error");
		exit(1);
	}

	dfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if(dfd < 0)
	{		
		perror("open error");
		close(sfd);
		exit(1);
	}

	int len;
	int ret;
	int pos;

	char buf[BUFSIZE];

	while(1)
	{
		len = read(sfd, buf, BUFSIZE);
		if(len < 0)
		{
			perror("read error");
			break;	
		}
		else if(len == 0)
		{
			break;
		}
	
		pos = 0;
		while(len > 0)
		{
			ret = write(dfd, buf + pos, len);
		
			if(ret < 0)
			{
				perror("write error");
				exit(1);
			}
			
			pos += ret;
			len -= ret;	
		}
	}

	close(dfd);
	close(sfd);

	return 0;
}
