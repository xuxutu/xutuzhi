#ifndef __APPLYCHFRAME__
#define __APPLYCHFRAME__
#include "spmanager.h"
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <stdio.h>
#include "command.h"
#include "loadmanifest.h"

using namespace std;

enum PhoneType{XH73Q, WOODY2, QD94R, QD26O};
static string entostr(PhoneType model);
extern PhoneType strtoenu(string model);
extern void loadfest(vector<string>& xml, map<string, string>& pro);

class Applych
{
public:
	Applych(){}

	Applych(string version, string date) : sp(version, date)
	{
		if(date[0])
		{
			workdir = "/home/zhaoyong/spwork/" + date + "/code/";
			pfilepath = "inputfile/" + date + "/";
		}
		else
		{	
			char m_date[32] = {0};
			workdir = "/home/zhaoyong/spwork/" + string(getDate(m_date)) + "/code/";
			pfilepath = "inputfile/" + string(getDate(m_date)) + "/";
		}	
	
		
	}

	//This interface is used to clone objects of specified type by customer.
	static Applych* findAndClone(PhoneType type, string date);

	//provide base build frame.
	void startApply();
protected:

	//Every object that inherit base class must overide this method since Only it know how to clone itself.
	virtual Applych* clone(string date) = 0;
	//Every object that inherit base class must overide this method to return a type that can uniquely identify self.
	virtual PhoneType returnType() = 0;

	//static function is used to operate static data.
	static void addPrototype(Applych* model)
	{
		models.push_back(model);
	}

	//create work dirctory
	virtual void makeWorkDir();
	//initialize the environment	
	virtual void initEnv() = 0;
	//Set the valid xml for get patch path
	virtual void setfest(){	manifests.push_back(string(".repo/manifest.xml"));cout<<"repo/manifest.xml"<<endl;}
	//get the path corresponding to the project
	void init_xml();

	//Decide whether the patch needs to be applied according user's needs. 
	void triatpatches();

	//change commit message
	void set_commit();

	//customize submit information
	virtual string set_message(string android_id, string patch_level);

	void syncode();

	//apply patch
	//create branch	
	virtual bool git_branch(ticket& tik);

	//apply
 	bool git_am(ticket& tik);

	//git push();
	virtual bool git_push(ticket& tik);

	bool git_abort(ticket& tik);

	void _applych(ticket& tik);
	void applych();

protected:
	SpManager sp;
	string workdir;
	string pfilepath;
	vector<string> commit_branch;
	string remote;					
	vector<string> manifests;
	map<string, string> pro;	
private:
	//This container include pointers of all models.
	static vector<Applych*> models;
	string commit_message;
};



#endif
