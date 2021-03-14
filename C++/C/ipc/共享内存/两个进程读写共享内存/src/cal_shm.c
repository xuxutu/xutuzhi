#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tell.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	//创建共享内存
	int shmid;
	if((shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT | IPC_EXCL | 0777)) < 0)
	{
		perror("shmget eror");
		exit(1);
	}

	pid_t pid;
	init();
	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid > 0)
	{
		//parent 
		//进行共享内存映射
		int* pi = (int*)shmat(shmid, 0, 0);
		if(pi == (int*)-1)
		{
			perror("shmat error");
			exit(1);
		}	
	
		//写入数据
		*pi = 100;
		pi[1] = 10;\
		//解除映射
		shmdt(pi);
		notify_pipe();//同知子进程读取数据	
		destroy_pipe();
		wait(0);

	}
	else
	{
		wait_pipe();//等待父进程写入数据	
		//读取数据
		//映射
		int* pi = (int*)shmat(shmid, 0, 0);
		if(pi == (int*)-1)
		{
			perror("shmat error");
			exit(1);
		}

		printf("%d %d\n", pi[0], pi[1]);	
		shmdt(pi);

		//删除共享内存，子进程不继承共享内存
		shmctl(shmid, IPC_RMID, NULL);	

		destroy_pipe();
	}
	
	return 0;
}
