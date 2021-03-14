#include "account.h"
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "pv.h"

int main()
{
	//在共享内存中创建账户
	int shmid;
	
	if((shmid = shmget(IPC_PRIVATE, sizeof(Account), IPC_CREAT | IPC_EXCL | 0777)) < 0)
	{
		perror("shmget error");
		exit(1);
	}

	//进行共享内存映射
	Account* a = (Account*)shmat(shmid, 0, 0);
	if(a == (Account*)-1)
	{
		perror("shmat error");
		exit(1);
	}

	a->code = 100001;
	a->balance = 10000;

	//创建信号量并初始化
	a->semid = I(1,1);
	if(a->semid < 0)
	{
		perror("I() error");
		exit(1);
	}

	printf("balance: %f\n", a->balance);

	pid_t pid;

	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid > 0)
	{
		//parent process
		//父进程取款
		double amt = withdraw(a, 10000);
		
		printf("pid: %d withdraw %f from code %d\n", getpid(), amt, a->code);

		wait(0);

		//解除映射之前操作
		printf("balance: %f\n", a->balance);
		
		//销毁信号量
		D(a->semid);

		shmdt(a);
	
		//释放共享内存
		shmctl(shmid, IPC_RMID, NULL);		
	}
	else
	{
		//child process
		double amt = withdraw(a, 10000);
		
		printf("pid: %d withdraw %f from code %d\n", getpid(), amt, a->code);

		shmdt(a);
	}

	exit(0);
}
