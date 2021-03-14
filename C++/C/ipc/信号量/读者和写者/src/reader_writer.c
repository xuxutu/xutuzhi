#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
typedef struct
{
	int val;
	int semid;

}Storage;


void init(Storage* s)
{

	assert(s != NULL);
	//创建信号量集,2个
	if((s->semid = semget(IPC_PRIVATE, 2, IPC_CREAT | IPC_EXCL | 0777)) < 0)
	{
		perror("semget error");
		exit(1);
	}	
	
	//对信号量集的所有信号量初始化
	union semun
	{
		int 					val;
		struct semid_ds*	 	ds; 
		unsigned short*			array;
	};

	union semun un;
	unsigned short array[2] = {0,0};
		
	un.array = array;
	if(semctl(s->semid, 0, SETALL, un) < 0)
	{
		perror("semctl error");
		exit(1);
	}

	

}


void destroy(Storage* s)
{
	assert(s != NULL);
	if(semctl(s->semid, 0, IPC_RMID, NULL) < 0)
	{
		perror("semctl error");
		exit(1);
	}
}


void Write(Storage* s,int val)
{
	//写入数据
	s->val = val;
	printf("%d write %d\n", getpid(), val);
	//设置V1操作 0号
	struct sembuf ops_v[1] = {{0, 1, SEM_UNDO}};
	//设置P1操作 1号
	struct sembuf ops_p[1] = {{1, -1, SEM_UNDO}};

	//p,v操作 1表示结构体个数	
	if(semop(s->semid, ops_v, 1) < 0)
	{
		perror("semop error");
	}
	
	if(semop(s->semid, ops_p, 1) < 0)
	{
		perror("semop error");

	}	

}


void Read(Storage* s)
{
	assert(s != NULL);
	
	//设置1号信号量作v1操作
	struct sembuf ops_v[1] = {{1, 1, SEM_UNDO}};
	//设置0号信号量作p1操作
	struct sembuf ops_p[1] = {{0, -1, SEM_UNDO}};
	
	if(semop(s->semid, ops_p, 1) < 0)
	{
		perror("semop error");

	}

	//读取数据
	printf("%d read %d\n", getpid(), s->val);

		
	if(semop(s->semid, ops_v, 1) < 0)
	{
		perror("semop error");
	}

	
}


int main()
{

	//将共享资源创建在共享内存当中
	int shmid;
	if((shmid = shmget(IPC_PRIVATE, sizeof(Storage), IPC_CREAT | IPC_EXCL | 0777)) < 0)
	{
		perror("shmget error");
		exit(1);
	}

	//父进程进行内存映射
	Storage* s = (Storage*)shmat(shmid, 0, 0);
	if(s == (Storage*)-1)
	{
		perror("shmat error");
		exit(1);	
	}
	
	//创建信号量并初始化
	init(s);

	pid_t pid;
	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid > 0)
	{
		//parent
		for(int i = 1; i <= 100 ; i++)
		{
			Write(s, i);
		}
		 	
		wait(0);
		destroy(s);
		shmdt(s);
			
		shmctl(shmid, IPC_RMID, NULL);
	}
	else
	{
		//child
		for(int i = 1; i <= 100; i++)
		{
			Read(s);
		}

		shmdt(s);
	}
	

	return 0;
}
