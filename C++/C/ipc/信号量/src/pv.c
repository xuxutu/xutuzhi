#include "pv.h"
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
union semun
{
	int 				val;	
	struct semid_ds* 	buf;
	unsigned short* 	array; 
};

//初始化semnums个信号灯的值(value)
int I(int semnums, int value)
{
	//创建信号量集
	int semid = semget(IPC_PRIVATE, semnums, IPC_CREAT | IPC_EXCL | 0777);
	if(semid < 0)
	{
		perror("semget error");
		return -1;
	}

	union semun un;
	unsigned short* array = (unsigned short*)calloc(semnums, sizeof(unsigned short));
		
	int i;
	for(i = 0; i < semnums; i++)
	{
		array[i] = value;
	}

	un.array = array;

	//初始化信号灯，0代表所有

	if(semctl(semid, 0, SETALL ,un) <0)
	{
		perror("semctl error");
		return -1;
	}

	free(array);

	return semid;
}

//一般为减一操作
void P(int semid, int semnum, int value)
{
	assert(value >= 0);
	//定义sembuf结构体数组，放置若干个结构体变量
	//对应要操作的信号量、要作的P或V操作
	struct sembuf ops[] = {{semnum, -value, SEM_UNDO}};		
	
	if(semop(semid, ops, sizeof(ops) / sizeof(struct sembuf)) <0)
	{
		perror("semop error");
		exit(-1);
	}
}

//一般为加一操作
void V(int semid, int semnum, int value)
{

	assert(value >= 0);
	//定义sembuf结构体数组，放置若干个结构体变量
	//对应要操作的信号量、要作的P或V操作
	struct sembuf ops[] = {{semnum, value, SEM_UNDO}};		
	
	if(semop(semid, ops, sizeof(ops) / sizeof(struct sembuf)) <0)
	{
		perror("semop error");
		exit(1);
	}

}
//销毁信号量集
void D(int semid)
{
	if(semctl(semid, 0, IPC_RMID, NULL) < 0)
	{
		perror("semctl error");
	}
}


