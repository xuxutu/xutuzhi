#include "applychlist.h"

//XH73Q---------------------------------------------
//every model should set verison.
string Applych_XH73Q::version = "10.0.0";

Applych_XH73Q Applych_XH73Q::APPLYCH_XH73Q;//allocate memory.

int Applych_XH73Q::count;//allocate memory.



//if necessary,here can customize the submission information. 
string Applych_XH73Q::set_message(string android_id, string patch_level)
{
	string a("NDROID");
	android_id.insert(1,a);	
	return "[OF6Q-572]:Security Patch " + patch_level + " " + android_id;
}


void Applych_XH73Q::initEnv()
{
	cout<<"3.init Enviroment"<<endl;
	SinCmd init_cmd("repof init -u ssh://zhaoyong0602@10.74.185.240:29420/FIH/QC/manifest -b OF6/dev/SDM636 -m 30100013_SP_BUILD.xml", workdir);
	//repof init -u ssh://zhaoyong0602@10.74! -b OF6/dev/SDM636 -m 30100013_SP_BUILD.xml
	init_cmd.excute();
}


void Applych_XH73Q::setfest()
{	
	cout<<"4.push_back all .xml releated to project"<<endl;
	manifests.push_back(".repo/manifests/30100013_SP/SDM660.xml");
	manifests.push_back(".repo/manifests/30100013_SP/MDM.xml");
	manifests.push_back(".repo/manifests/30100013/QC.xml");
	manifests.push_back(".repo/manifests/30100013_SP/BSP.xml");
	manifests.push_back(".repo/manifests/30100013/PFAQ.xml");
}


bool Applych_XH73Q::git_branch(ticket& tik)
{
	int flag1 = 0, flag2 = 0;
	auto pos = pro.find(tik.project); 
	string workpath = workdir + "/" + pos->second;
    SinCmd cmd_git_fetch1("git fetch "+ remote + " refs/heads/" + commit_branch[0], workpath);
    if(cmd_git_fetch1.excute())
	{
		tik.inbranch = commit_branch[0];
		flag1 = 1;	
		cout<<"this project belong to "<<commit_branch[0]<<endl;
		SinCmd cmd_git_branch("git checkout " + remote + "/" + commit_branch[0] + " -b MASTER");
		if(!cmd_git_branch.excute())
		{
			cout<<"git branch failed!"<<endl;
			//exit(-1);
		}	
	}
	SinCmd cmd_git_fetch2("git fetch "+ remote + " refs/heads/" + commit_branch[1], workpath);
    if(cmd_git_fetch2.excute())
    {   
		tik.inbranch = commit_branch[1];
		flag2 = 1;
		cout<<"this project belong to "<<commit_branch[1]<<endl;
		SinCmd cmd_git_branch("git checkout " + remote + "/" + commit_branch[1] + " -b MASTER");
		if(!cmd_git_branch.excute())
		{
			cout<<"git branch failed!"<<endl;
			//exit(-1);
		}
	}

	if(flag1 && flag2)
	{
		if(tik.project == "platform/frameworks/base")
		{
			SinCmd cmd_git_branch("git checkout " + remote + "/OF6/dev/SDM660/30100013/PFAQ -b Base_Master");
			cmd_git_branch.excute();
			tik.inbranch = "OF6/dev/SDM660/30100013/PFAQ";
			cout<<"warning : this project :"<<tik.project<<" exists under both remote branches."<<endl;
		}
		else if(tik.project == "platform/system/bt")
		{
			tik.isApply = 0;
			cout<<"warning : this project :"<<tik.project<<" exists under both remote branches."<<endl;	
		}
		else
		{
			cout<<"warning : this project :"<<tik.project<<" exists under both remote branches."<<endl;
			exit(-1);
		}
	}

	if(flag1 || flag2)
		return 1;
	else	
		return 0;	
}

//WOODY2-------------------------------------------------------
string Applych_WOODY2::version = "8.1.0";

Applych_WOODY2 Applych_WOODY2::APPLYCH_WOODY2;

void Applych_WOODY2::initEnv()
{
	cout<<"3.init Enviroment"<<endl;
	SinCmd init_cmd("repog init -u ssh://zhaoyong0602@10.74.185.240:29420/sharp/SPF_301_O/offshore/manifest -b WOODY2_O_MASTER", workdir);
	init_cmd.excute();
}

//QD94R--------------------------------------------------------
string Applych_QD94R::version = "11.0.0";

Applych_QD94R Applych_QD94R::APPLYCH_QD94R;

void Applych_QD94R::initEnv()
{
	cout<<"3.init Enviroment"<<endl;
	SinCmd init_cmd("repog init -u ssh://zhaoyong0602@10.74.185.240:29420/sharp/PCQ/offshore/manifest -b R/SM8150/MASTER -m p4d.xml --repo-branch=stable_sharp", workdir);
	init_cmd.excute();
}

//QD26O-----------------------------------------------------------
