#ifndef __COMMAND__
#define __COMMAND__
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

class Command
{
public:	
	Command(){}
	Command(const string cmd, const string workdir = NULL) : m_workdir(workdir), m_cmd(cmd)
	{
		exeName = cmd.substr(0, cmd.find_first_of(' '));
	}
	
	void mysystem(const char* cmd);
	
	virtual void excute() = 0;

protected:
	const string m_workdir;
	const string m_cmd;
	string exeName;	
};



void Command::mysystem(const char* cmd)
{			
	pid_t pid;
	
	if((pid = fork()) < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid == 0)
	{
		//puts(getcwd(NULL, 0));
		//puts(cmd);	
		if(execlp("bash", "bash", "-c", cmd, NULL) < 0)
		{
			fprintf(stderr, "%s excute error: %s\n",exeName.c_str(), strerror(errno));
			//perror("execvp error");
			exit(1);
		}

	}	
	else
	{
		wait(0);
	}
}


class SinCmd : public Command
{
public:	
	SinCmd(const string cmd, const string workdir = getcwd(NULL, 0)) : Command(cmd, workdir){}

	void excute()
	{
		int err = chdir(m_workdir.c_str());
		if(err)
		{
			perror("chdir error");
			puts(getcwd(NULL, 0));	
			exit(1);
		}
		

		mysystem(m_cmd.c_str());
		//system(m_cmd.c_str());		
	}

};

class MutiCmd : public Command
{
public:
	void excute()			
	{
		for(int i = 0; i < cmds.size(); i++)
		{
			cmds[i]->excute();
		}
	}

	void addCmd(Command* cmd)
	{
		cmds.push_back(cmd);
	}

private:
	vector<Command*> cmds;
};

#endif
