#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DELIMS " \t\n"


struct cmd_st
{
	glob_t globres;
};

static void prompt(void)
{
	printf("mysh-0.1$ ");
}


static void parse(char* line, struct cmd_st* res)
{
	char* tok;
	int i = 0;	


	while(1)
	{
		tok = strsep(&line, DELIMS);
		if(tok == NULL)
			break;

		//多个分隔符会产生空串
		if(!tok[0])	
			continue;

		glob(tok, GLOB_NOCHECK | GLOB_APPEND * i, NULL, &res->globres);
			i = 1;
		
	}
}

//cd 属于内部命令 切换运行路径 所以目前不适用

int main(int argc , char* argv[])
{
	char*  linebuf = NULL;
	size_t linebuf_size = 0;
	struct cmd_st cmd;
	pid_t pid;

	while(1)
	{
		prompt();
		if(getline(&linebuf, &linebuf_size, stdin) < 0)
			break;
		parse(linebuf, &cmd);
		
		if(0) //内部命令待实现
		{
			//do sth	
		}
		else
		{
			if((pid = fork()) < 0)
			{
				perror("fork error");
				exit(1);
			}
			if(pid == 0)
			{
				execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
				perror("execvp error");
			}
			else
			{
				wait(NULL);
			}	
		}
	}



	return 0;
}

