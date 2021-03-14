#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

char* cmd1[3] = {"cat", "答案", NULL};
char* cmd2[3] = {"grep", "爱情", NULL};

int main(void)
{
	int fd[2];
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
				if(dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)		
				{
					perror("dup2 error");
				}

				close(fd[1]);

				//执行cat
				if(execvp(cmd1[0], cmd1) < 0)
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
				if(execvp(cmd2[0], cmd2) < 0)
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


	return 0;	
}
