#ifndef __APPLYCHLIST__
#define __APPLYCHLIST__
#include "applychframe.h"



class Applych_XH73Q : public Applych
{
protected:
	Applych_XH73Q(string date) : Applych(version, date)
	{
		//指定提交的branch
		commit_branch.push_back("OF6/dev/SDM660/30100013/BSP");
		commit_branch.push_back("OF6/dev/SDM660/30100013/PFAQ");

		//指定remote
		remote = "QC";
	}

public:
	Applych* clone(string date){ return new Applych_XH73Q(date); }

	PhoneType returnType(){ return XH73Q;}

	string set_message(string android_id, string patch_level);
	void initEnv();
	void setfest();
	bool git_branch(ticket& tik);
private:

	//SpManager sp;

	static Applych_XH73Q APPLYCH_XH73Q;
	Applych_XH73Q()
	{
		addPrototype(this);
	}
	
	static string version;
	int _id;
	static int count;
};

class Applych_WOODY2 : public Applych 
{
protected:	
	Applych_WOODY2(string date) : Applych(version, date)
	{
		//指明提交仓库
		remote = "WOODY2_O_MASTER";				
	}
public:
	Applych* clone(string date){ return new Applych_WOODY2(date); }
	PhoneType returnType(){ return WOODY2;}
	void initEnv();
private:	
	static Applych_WOODY2 APPLYCH_WOODY2;
	Applych_WOODY2()
	{
		addPrototype(this);
	}

	static string version;	
};


class Applych_QD94R : public Applych 
{
protected:
	Applych_QD94R(string date) : Applych(version, date)
	{
		//指明提交仓库
		remote = "R/AQUOS/MASTER";
	}
public:
	Applych* clone(string date){ return new Applych_QD94R(date); }
	PhoneType returnType(){ return QD94R;}
	void initEnv();
private:
	static Applych_QD94R APPLYCH_QD94R;
	Applych_QD94R()
	{
		addPrototype(this);
	}
	static string version;
};


class Applych_QD26O : public Applych
{



};


#endif
