#include "applychframe.h"

static string entostr(PhoneType model)
{
	if(model == XH73Q) return string("XH73Q");
	if(model == WOODY2) return string("WOODY2");
	if(model == QD94R) return string("QD94R");
	if(model == QD26O) return string("QD26O");
}


PhoneType strtoenu(string model)
{
	if(model == "XH73Q") return XH73Q;
	if(model == "WOODY2") return WOODY2;
	if(model == "QD94R") return QD94R;
	if(model == "QD26O") return QD26O;
}


vector<Applych*> Applych::models; //this step allocate memory for models.

Applych* Applych::findAndClone(PhoneType type, string date)
{	
	for(int i = 0; i < models.size(); i++)
	{
		if(models[i]->returnType() == type)
		{
			return models[i]->clone(date);
		}
	}
}

void Applych::makeWorkDir()
{

	workdir += entostr(returnType());	
	SinCmd cmd("mkdir -p " + workdir);	
	cout<<"1.create workdir: "<<workdir<<endl;
	cmd.excute();

}


void Applych::init_xml()
{
	//load manifest
	cout<<"5.get project->name project->path"<<endl;
/*
	vector<string> projects = sp.getprojects();	

	loadfest(manifests, projects, pro);

	if(pro.size() != projects.size())
	{
		cout<<"warning:some projects->path get failed!"<<endl;
		//exit(-1);
	}
*/
	loadfest(pro, manifests);
}	

void Applych::triatpatches()
{
	cout<<"6.triat patches"<<endl;
	for(auto& tik : sp.getTickets())
	{
		auto pos = pro.find(tik.project);
		if(pos != pro.end())
		{
			tik.isApply = 1;
		}
		else
		{	
			tik.isApply = 0;
			cout<<"project: "<<tik.project<<"get apropriate path failed in xml"<<endl;
			cout<<"CVE	  : "<<tik.cve<<endl;
			cout<<"android id : "<<tik.android_id<<endl;	
		}

	}

	
	
}

string Applych::set_message(string android_id, string patch_level)
{
	string a("NDROID");
	android_id.insert(1,a);
	return "セキュリティパッチ " + patch_level + " " + android_id;
}



static void write_message(int fd, const char* message, int len)
{
	int ret;
	int pos = 0;
	
	while(len > 0)
	{
		ret = write(fd, message + pos, len);

		if(ret < 0)
		{
			perror("write error");
			exit(-1);
		}

		pos += ret;

		len -= ret; 
	}

}


static void _set_commit(ticket& tik, string message, string pfilepath)
{
	//1.open file
	//cout<<message.size()<<endl;

	int msize = message.size() + 1 + 1 + strlen("[porting]") + 1 + 1 + 1 + tik.cve.size() + 1 + 1;
	
	string patchfile = pfilepath + tik.patchfile;


	int sfd, dfd;
	sfd = open(patchfile.c_str(), O_RDONLY);
	if(sfd < 0)
	{
		fprintf(stderr, "open error : file :%s\n", tik.patchfile.c_str());
		exit(-1);
	}

	int pos2 = tik.patchfile.find("/0");
	int pos1 = tik.patchfile.find(tik.version);
	string dpath;
	if(pos1 != -1 && pos2 != -1)
	{
		dpath = pfilepath + "applyches/" + string().append(tik.patchfile, pos1, pos2-pos1);
	}	
	else
	{
		cout<<"destination path get error"<<endl;
		exit(-1);
	}	
	//cout<<dpath<<endl;

	//create destination path
	SinCmd mkdir_cmd("mkdir -p " + dpath);

	if((access(dpath.c_str(), F_OK)) == -1)   
   	{   
		mkdir_cmd.excute();
    }


	//初始化patchfile
	pos1 = tik.patchfile.find("/0");
	pos2 = tik.patchfile.size();
	if(pos1 == -1 || pos1 > pos2)
	{	
		puts("get \"platform\" failed");
		exit(-1);
	}

	tik.patchfile = string(getcwd(NULL, 0)) + "/" + dpath  + string().append(tik.patchfile, pos1, pos2-pos1);

	//open destination patchfile	
	dfd = open(tik.patchfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if(dfd < 0)
	{
		fprintf(stderr, "open error : %s, file :%s\n", strerror(errno), tik.patchfile.c_str());
		exit(-1);
	}

	char* line = NULL;
	size_t len = 0;
	ssize_t nread;


	int flag1 = 0; 
	int flag2 = 0;
	
	FILE* fp = fdopen(sfd, "r");
	if(fp < 0) perror("fdopen error");
	
	while((nread = getline(&line, &len, fp)) != -1)
	{
		if(strstr(line, "Subject: [PATCH] "))
		{
			string firstline = string(line + strlen("Subject: [PATCH] "));
			message = "Subject: [PATCH] " + message + "\n";
			write_message(dfd, message.c_str(), message.size());
			write_message(dfd, "\n", 1);
			write_message(dfd, "[porting]", strlen("[porting]"));
			write_message(dfd, "\n", 1);
			write_message(dfd, "\n", 1);
			write_message(dfd, firstline.c_str(), firstline.size());

			flag1 = 1;
			continue; 
		}

		if(flag1 && strstr(line, "Change-Id: "))
		{
			write_message(dfd, "\n", 1);
			write_message(dfd, (tik.cve + "\n").c_str(), tik.cve.size() + 1);
			write_message(dfd, "\n", 1);
			flag2 = 1;
		}

		write_message(dfd, line, strlen(line));
	}

	int src = getFileSize(patchfile.c_str());
	int des = getFileSize(tik.patchfile.c_str());
	int dis = des - src;
	
	//cout<<"dis = "<<dis<<endl;
	//cout<<"msize = "<<msize<<endl;	

	bool r = flag1 && flag2 && (dis == msize);
	if(!r)
	{
		fprintf(stderr, "patchfile: %s format error\n", patchfile.c_str());
		exit(-1);	
	}	


	free(line);
	fclose(fp);
	close(sfd);
	close(dfd);

	return;
}



void Applych::set_commit()
{
	
	for(auto& tik : sp.getTickets())
	{
		_set_commit(tik, set_message(tik.android_id, tik.patch_level), pfilepath);
		//cout<<tik.patchfile<<endl;
	}

	//string message = set_message(android_id, spl);

		
	cout<<"2.change commit message"<<endl;
}


bool Applych::git_abort(ticket& tik)
{
	auto pos = pro.find(tik.project);
	string workpath = workdir + "/" + pos->second;
	SinCmd cmd_git_abort("git am --abort", workpath);
	cmd_git_abort.excute();
	cmd_git_abort.go_back_dir();

	return true;
}

bool Applych::git_am(ticket& tik)
{

	auto pos = pro.find(tik.project);
	string workpath = workdir + "/" + pos->second;
	cout<<"apply path :"<<workpath<<endl;
	SinCmd cmd_git_am("git am " + tik.patchfile, workpath);

	if(cmd_git_am.excute())
	{
		cout<<"git am succeed!"<<endl;
		cmd_git_am.go_back_dir();
		return true;
	}

	cmd_git_am.go_back_dir();
	cout<<"git am failed!"<<endl;
	return false;
}

bool Applych::git_push(ticket& tik)
{
	auto pos = pro.find(tik.project);
	string workpath = workdir + "/" + pos->second;
	
	SinCmd cmd_git_push("repog upload .", workpath);
	string push = "git push ssh://LX19060027@10.24.71.180:29418/" + tik.project + " HEAD:refs/for/" +remote;
	
	//string push = "repog upload .";
	char** cmd = setargv(push);
	printcmd(cmd);
	//create pipe
	int fd[2];
	if(pipe(fd) < 0)
	{
		perror("pipe error");
		exit(1);
	}

	int i = 0;
	pid_t pid;

	int status;	
	pid_t exeid;

	for(; i < 2; i++)
	{
		//create child process
		pid = fork();
		if(pid < 0)
		{
			perror("fork error");
			exit(1);
		}	
		else if(pid == 0)
		{
			//first child process write data to pipe
			if(i == 0)
			{	
				exeid = getpid();
				//close read endian
				close(fd[0]);

				//stderr->pipe
				if(dup2(fd[1], STDERR_FILENO) != STDERR_FILENO)
				{
					perror("dup2 error");
				}

				close(fd[1]);

				//excute git push
				if(execvp(cmd[0], cmd) < 0)
				{
					perror("execvp error");
					exit(-1);
				}			
				
				break;	
			}

			if(i == 1)
			{
				//clode write endian
				close(fd[1]);
				//标准输入->fd[0]
						
				char buf[2048] = {0};
				
				char* p1;
				while(read(fd[0], buf, 2048))
				{
					//fprintf(stderr, "%s", buf);
					if((p1 = strstr(buf, "https:")) || (p1 = strstr(buf, "No changes between prior commit")))
					{
						cout<<tik.cve<<endl;
						cout<<tik.android_id<<endl;
						cout<<tik.patchfile<<endl;
						fprintf(stderr, "%s", p1);
					}
		
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
				waitpid(exeid, &status, 0);
				wait(0);
				cmd_git_push.go_back_dir();
				myfree(cmd);

				return sysret(status);
			}	
		}

	}
		
	return sysret(status);
}

void Applych::syncode()
{
	cout<<"7.sync code"<<endl;
	/*for(auto& it : pro) 
	{
		string workpath = workdir + "/" + it.second;
		SinCmd cmd_sync("repog sync -c -j4 --no-tags " + it.first);
		cmd_sync.excute();
		cmd_sync.go_back_dir();
	}*/

	for(auto& p : sp.getprojects())
	{
		auto pos = pro.find(p);
		if(pos != pro.end())
		{
			string workpath = workdir + "/" + pos->second;
			SinCmd cmd_sync("repog sync -c -j4 --no-tags " + pos->first);
			cmd_sync.excute();
			cmd_sync.go_back_dir();	
		}
		else
		{
			cout<<"this project not exist in xml->"<<p<<endl;
			exit(-1);	
		}	
	}
}


bool Applych::git_branch(ticket& tik)
{
	auto pos = pro.find(tik.project);
	string workpath = workdir + "/" + pos->second;
	SinCmd cmd_git_branch("repog start " + tik.cve + " .", workpath);
	cmd_git_branch.excute();
	cmd_git_branch.go_back_dir();

	return 1;		
}	
	
void Applych::_applych(ticket& tik)
{
	if(tik.isApply)
	{
		if(!git_branch(tik))
		{
			cout<<"git branch failed : "<<tik.inbranch<<endl;
			exit(-1);
		}

		if(git_am(tik))
		{
			//if(!git_push(tik))
				//cout<<"git push failed"<<endl;
			cout<<"git push succeed"<<endl;
		}
		else
		{
			git_abort(tik);
		}
	}
	cout<<"-----------------------------------------------------------"<<endl;
}

void Applych::applych()
{
	cout<<"8.applych patches"<<endl;
	for(auto& tik : sp.getTickets())
	{
		_applych(tik);
		//cout<<tik.patchfile<<endl;
	}
}


void Applych::startApply()
{
	makeWorkDir();
	set_commit();
	initEnv();//change workdir->spwork/code/202102/XH73Q
	setfest();
	init_xml();//init project in spwork/code/202102/XH73Q/
	triatpatches();
	syncode();
	applych();
}

