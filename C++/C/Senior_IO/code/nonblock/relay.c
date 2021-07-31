#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define TTY1 "1"
#define TTY2 "2"

#define BUFSIZE 1024


enum STATE{STATE_R = 1, STATE_W, STATE_Ex, STATE_T};



//Finite state machine
struct fsm_st
{
	int state;
	int sfd;
	int dfd;
	char buf[BUFSIZE];
	int len;
	int pos;
	char* errstr;	
};


static void fsm_driver(struct fsm_st* fsm)
{
	int ret;

	switch(fsm->state)
	{
		case STATE_R:
		{
			fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
			if(fsm->len == 0)
				fsm->state = STATE_T;
			else if(fsm->len < 0)
			{
				if(errno == EAGAIN)
					fsm->state = STATE_R;
				else
				{
					fsm->errstr = "read error";
					fsm->state = STATE_Ex;
				}
			}
			else
			{	
				fsm->pos = 0;
				fsm->state = STATE_W;
			}
		}
		break;

		case STATE_W:
		{
			ret = write(fsm->dfd, fsm->buf + fsm->pos, fsm->len);
			if(ret < 0)
			{
				if(errno == EAGAIN)
					fsm->state = STATE_W;
				else
				{
					fsm->errstr = "write error";
					fsm->state = STATE_Ex;	
				}
			
			}
			else
			{
				fsm->len -=	ret;
				fsm->pos += ret;

				if(fsm->len)
				{
					fsm->state = STATE_W;
				}
				else
					fsm->state = STATE_R;
				
			}

		}	
		
		case STATE_Ex:
		{
			fsm->state = STATE_T;
			perror(fsm->errstr);
		}
		break;
		
		case STATE_T:
		{
			/*do sth*/
		}
		break;

		default:
			abort();
			break;
	}
}


static void relay(int fd1, int fd2)
{
	int fd1_save, fd2_save;
	struct fsm_st fsm12, fsm21;
	

	//获取状态，改为非阻塞
	fd1_save = fcntl(fd1, F_GETFL);
	fcntl(fd1, F_SETFL, fd1_save | O_NONBLOCK);

	fd2_save = fcntl(fd2, F_GETFL);
	fcntl(fd2, F_SETFL, fd2_save | O_NONBLOCK);
	

	//状态机1
	fsm12.state = STATE_R;
	fsm12.sfd = fd1; 
	fsm12.dfd = fd2;

	
	//状态机2
	fsm21.state = STATE_R;
	fsm21.sfd = fd2;
	fsm21.dfd = fd1;

	//推状态机
	while(fsm12.state != STATE_T || fsm21.state != STATE_T)
	{
		fsm_driver(&fsm12);
		fsm_driver(&fsm21);
	}


	//恢复
	fcntl(fd1, F_SETFL, fd1_save);
	fcntl(fd2, F_SETFL, fd2_save);

}


int main()
{
	int fd1, fd2;	

	fd1 = open(TTY1, O_RDWR);
	if(fd1 < 0)
	{
		perror("open error");
		exit(1);
	}

	//write(fd1, "TTY11:\n", strlen("TTY11:\n") + 1);


	fd2 = open(TTY2, O_RDWR | O_NONBLOCK);
	if(fd2 < 0)
	{
		perror("open error");
		exit(1);
	}	

	//write(fd2, "TTY12:\n", strlen("TTY12:\n") + 1);

	relay(fd1, fd2);

	close(fd1);
	close(fd2);

	return 0;
}

