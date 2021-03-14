#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


//定义进程的终止函数
void term_fun1(void)
{
	printf("first term function\n");
}

void term_fun2(void)
{
	printf("second term function\n");
}

void term_fun3(void)
{
	printf("third term function\n");
}


int main(int argc,char* argv[])
{

	if(argc < 3)
	{
		fprintf(stderr,"usage: %s file [exit|_exit|return]\n",argv[0]);
		exit(1);
	}
	
	//向内核登记终止函数
	atexit(term_fun1);
	atexit(term_fun2);
	atexit(term_fun3);

	//写入文件
	FILE* pf=fopen(argv[1],"w");

	fprintf(pf,"hello totek");//全缓存，return/exit会清缓存

	if(!strcmp(argv[2],"exit"))
	{
		exit(0);//标准c库函数，会调用终止函数
	}
	else if(!strcmp(argv[2],"_exit"))
	{
		_exit(0);//系统调用，不会调用终止函数，不会清缓存
	}
	else if(!strcmp(argv[2],"return"))
	{
		return 0;
	}
	else
	{
		fprintf(stderr,"usage: %s file [exit|_exit|return]\n",argv[0]);
	}	
}

