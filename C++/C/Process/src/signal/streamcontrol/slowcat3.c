#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

#define CPS 100
#define BUFSIZE CPS
static volatile int loop = 0;



void sigalrm_handler(int s)
{
	//alarm(1);
	loop = 1;
}

int main(int argc, char** argv)
{
	struct itimerval itv;

	if(argc < 2)
	{
		fprintf(stderr, "usage: %s desfile\n", argv[0]);
		exit(1);
	}	

	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;

	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;

	//alarm(1);
	if(setitimer(ITIMER_REAL, &itv, NULL) < 0)
	{
		perror("setitmer failed:");
		exit(1);
	}


	signal(SIGALRM, sigalrm_handler);


	int sfd, dfd = 1;


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
		while(!loop)
			pause();

		loop = 0;

		while((len = read(sfd, buf, BUFSIZE)) < 0)
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

	return 0;
}
