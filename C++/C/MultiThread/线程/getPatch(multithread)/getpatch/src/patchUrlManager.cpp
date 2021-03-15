#include"patchUrlmanager.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
PatchUrlManager::PatchUrlManager(const char* cr): m_cr(cr)
{
	filename="inputfile/ALL_URL_"+m_cr+".txt";
	FILE* pf=fopen(filename.c_str(),"r+");

	if(!pf)
	{
		cout<<filename<<endl;
		puts("文件打开失败");
	}

	char buf[1024]={0};

	while(fgets(buf,sizeof(buf),pf))
	{
		//去掉换符号
		*strchr(buf,'\n')=0;
		m_purl.push_back(buf);
		//cout<<buf<<endl;
		getPath(buf);
	}
}

void PatchUrlManager::showCr()
{

}

const string PatchUrlManager::getPath(string str)
{
	char buf[1024]={0};
	strcpy(buf,str.c_str());
	char* p1;
	char* p2;
	if(p1=strstr(buf,"https://source.codeaurora.org/"))
	{
		p1=p1+strlen("https://source.codeaurora.org/");
		if(p2 = strstr(buf, "patch"))
		{
			string path(p1,p2);
			//cout<<path<<endl;
			return path; 
		}
	}	
}

const string PatchUrlManager::getCommit(string str)
{
	char buf[1024]={0};
	strcpy(buf,str.c_str());
	char* p=strchr(buf,'=')+1;	
	string stri=p;
	return stri;
}

void PatchUrlManager::showUrl()
{
	



}

void mysystem(const char* cmd)
{

			
	pid_t pid;
	
	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid == 0)
	{
		if(execlp("bash", "bash", "-c", cmd, NULL) < 0)
		{
			perror("execvp error");
			exit(1);
		}
	}	
	else
	{
		wait(0);
	}
	
/*	int fd[2];
	if(pipe(fd) < 0)
	{
		perror("pipe error");
		exit(1);
	}

	int i = 0;
	pid_t pid;

	for(; i < 2; i++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork error");
			exit(1);
		}
		else if(pid == 0)
		{
			//child process	
			if(i == 0)
			{
				//第一个子进程,负责往管道写入数据
				//关闭读端
				close(fd[0]);

				//标准输出->管道写端
				if(dup2(fd[1], STDERR_FILENO) != STDERR_FILENO)		
				{
					perror("dup2 error");
				}

				close(fd[1]);

				if(execlp("bash", "bash", "-c", cmd, NULL) < 0)
				{
					perror("execvp error");
					exit(1);
				}

				break;
			}
			if(i == 1)
			{
				//第二个子进程，负责从管道读取数据
				//关闭写端
				close(fd[1]);
				
				//标准输入->fd[0]
					
				if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
				{
					perror("dup2 error");
				}	
				
				close(fd[0]);
				//执行grep

				if(execlp("bash", "bash", "-c", "grep 已保存", NULL) < 0)
				{	
					perror("execvp error");
					exit(1);
				}

				break;
			}
						
		}
		else
		{
			if(i == 1)
			{
				close(fd[0]);
				close(fd[1]);
			
				//parent process
				wait(0);
				wait(0);	
			}	

			
		}
	}

*/









}


void* PatchUrlManager::__downpatch(void* command)
{
	Cmd cmd = *(Cmd*)command;

	mysystem(cmd.cmd1.c_str());
	mysystem(cmd.cmd2.c_str());


	
	return (void*)0;
}


void PatchUrlManager::downpatch()
{
	int err;	

	int count=0;
	
	//打开文件输入log
	string filename = m_cr + ".log";
	int fdout = open(filename.c_str(), O_WRONLY|O_CREAT, 0777);
	if(fdout < 0)
	{
		fprintf(stderr, "open error: %s\n", strerror(errno));
		exit(1);
	}				
			

	pthread_t th[m_purl.size()];
	//定义线程属性	
	pthread_attr_t attr[m_purl.size()];	
	
	for(int i = 0; i < m_purl.size(); i++)		
	{
		//对线程属性初始化
		pthread_attr_init(attr + i);
		//设置分离属性为分离状态启动
		pthread_attr_setdetachstate(attr + i, PTHREAD_CREATE_DETACHED);
		//以分离状态启动线程	
		
		string path = "kerpatches/"+m_cr+"/"+getPath(m_purl[i]);
		cmd.cmd1 = "mkdir -p " + path;
		cmd.cmd2 = "wget -O "+path+m_cr+"_"+getCommit(m_purl[i])+".patch "+m_purl[i];
		cmd.cmd3 = "rm "+path+m_cr+"_"+getCommit(m_purl[i])+".patch";		
		ssize_t len = strlen(m_purl[i].c_str());
		const char* buffer = m_purl[i].c_str();
		if((err = write(fdout, buffer, len)) != len)
		{
			perror("write error");
			exit(1);
		}

		write(fdout, "\n", 1);		
			
		if((err = pthread_create(th+i, attr + i, PatchUrlManager::__downpatch, &cmd)) != 0)		
		{
			perror("pthread_create error");
		}

		//销毁线程属性
		pthread_attr_destroy(attr + i);			
	}		

	//
	close(fdout);

}
