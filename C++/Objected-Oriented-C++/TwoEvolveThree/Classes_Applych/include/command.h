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
#include <stack>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

extern int sysret(pid_t status);

class Command
{
public:	
	Command(){}
	Command(const string cmd, const string workdir = NULL) : m_workdir(workdir), m_cmd(cmd)
	{
		exeName = cmd.substr(0, cmd.find_first_of(' '));
	}
	
	int mysystem(string cmd);
	
	virtual int excute() = 0;

protected:
	const string m_workdir;
	const string m_cmd;
	stack<string> dir;
	string exeName;	
};

class SinCmd : public Command
{
public:	
	SinCmd(const string cmd, const string workdir = getcwd(NULL, 0)) : Command(cmd, workdir)
	{
		//将当前工作目录入栈
		dir.push(workdir);
	}

	int excute()
	{		
		int err = chdir(m_workdir.c_str());
		if(err)
		{
			perror("chdir error");
			puts(getcwd(NULL, 0));	
			exit(1);
		}
		
		cout<<m_cmd<<endl;;
		return sysret(system(m_cmd.c_str()));

		//system(m_cmd.c_str());		
	}

	void go_back_dir()
	{
		int err = chdir(dir.top().c_str());	
		if(err)
		{
			perror("chdir error");
			puts(getcwd(NULL, 0));	
			exit(1);
		}
	}
};

class MutiCmd : public Command
{
public:
	int excute()			
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




extern void myfree(char** cmd);
extern char** setargv(string cmd);
extern void printcmd(char** cmd);

#endif
