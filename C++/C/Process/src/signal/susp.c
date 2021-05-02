#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

void int_handler(int s)
{
	write(1, "!", 1);
}

int main()
{
	sigset_t set, oset, savaset;

	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	
	//我要保证我的模块不改变任何状态
	//sigprocmask(SIG_UNBLOCK, &set, &saveset)；//着重在获取改变之前的状态
	//sigprocmask(SIG_SETMASK, &saveset, NULL);

	//signal(SIGINT, SIG_IGN);
	signal(SIGINT, int_handler);


	sigprocmask(SIG_BLOCK, &set, &oset);	
	for(int j = 0; j < 1000; j++)
	{
		//sigprocmask(SIG_BLOCK, &set, NULL);	

		for(int i = 0; i < 5; i++)
		{
			write(1, "*", 1);
			sleep(1);
		}
		write(1, "\n", 1);
		sigset_t tmpset;

		sigsuspend(&oset);		
/*
		sigprocmask(SIG_SETMASK, &oset, &tmpset);
		pause();
		sigprocmask(SIG_SETMASK, &tmpset, NUL)
*/
		//sigprocmask(SIG_UNBLOCK, &set, NULL);
	}

	return 0;
}
