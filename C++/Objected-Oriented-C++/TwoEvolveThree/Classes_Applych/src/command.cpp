#include "command.h"

int sysret(pid_t status)	
{
    if (-1 == status)
    {
        printf("system error!\n");
		exit(-1);
    }
    else
    {
        //printf("exit status value = [0x%x]\n", status);
 
        if (WIFEXITED(status))
        {
            if (0 == WEXITSTATUS(status))
            {
                //printf("run shell script successfully.\n");
				return 1;	
            }
            else
            {
                //printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
				return 0;
            }
        }
        else
        {
			printf("excute exception! exit status = [%d]\n", WEXITSTATUS(status));
            //printf("exit status = [%d]\n", WEXITSTATUS(status));
			//exit(-1);
			return 0;
        }
	
    }
 
	printf("Excute ERROR\n");
}

char** setargv(string cmd)
{
	int pos = 0;	
	int n = 0;

	while((pos = cmd.find(' ',pos)) != -1)
	{
		pos++;
		n++;
	}		
	
	char** p1 = (char**)malloc(sizeof(char*) * (n + 2));
	if(!p1)
	{
		perror("malloc error:");
		exit(-1);
	}

	memset(p1, 0, sizeof(char*) * (n + 2));

	char* t = (char*)malloc(cmd.size() * sizeof(char) + 1);
	if(!t)
	{
		perror("malloc error:");
		exit(-1);
	}

	char* p2 = t;	
	const char* tmp = cmd.c_str();	
	strcpy(p2, tmp);	
	

	char* p3 = p2;
	int i = 0;
	while(p2 = strchr(p2, ' '))
	{
		*p2 = 0;
		p1[i] = (char*)malloc(sizeof(char) * strlen(p3) + 1);
		if(!p1[i])
		{
			perror("malloc error:");
			exit(-1);
		}
		strcpy(p1[i], p3);
		p3 = ++p2;
		i++;		
	}

	p1[i] = (char*)malloc(sizeof(char) * sizeof(p3) + 1);
	if(!p1[i])
	{
		perror("malloc error:");
		exit(-1);
	}
		
	
	strcpy(p1[i], p3);

	free(t);
	return p1;
}

void myfree(char** cmd)
{
	int i =0;
	while(cmd[i])
	{
		free(cmd[i]);
		//printf("%s ",cmd[i]);
		i++;
	}

	free(cmd);
}


void printcmd(char** cmd)
{
	int i =0;
	while(cmd[i])
	{
		printf("%s ",cmd[i]);
		i++;
	}
	cout<<endl;
}

int Command::mysystem(string cmd)
{
	char** c = setargv(cmd);

	printcmd(c);	
	pid_t pid;

	int status = 0;

	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(-1);
	}
	else if(pid == 0)
	{
		if(execvp(c[0], c) < 0)
		{
			fprintf(stderr, "%s excute error : %s\n", c[0], strerror(errno));
			myfree(c);
			exit(-1);
		}
			
	}
	else
	{
		wait(&status);
		/*if(status)
		{
			myfree(c);	
			//exit(-1);
		}
		else
		{
			myfree(c);
		}*/

	    if (WIFEXITED(status))
        {
            if (0 == WEXITSTATUS(status))
            {
                //printf("run shell script successfully.\n");
				myfree(c);
				return 1;
            }
            else
            {
                //printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
				myfree(c);
				return 0;
            }
        }
        else
        {
            printf("excute exception! exit status = [%d]\n", WEXITSTATUS(status));
			myfree(c);
			exit(-1);	
        }
		
		
	}		
	
	//int err = execvp(c[0],c);
	//fprintf(stderr, "%s excute error : %s\n", c[0], strerror(errno));
	//puts(getcwd(NULL,0));
}



