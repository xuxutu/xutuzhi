#ifndef __BUILDFRAME__
#define __BUILDFRAME__

#include "command.h"
#include <iostream>
#include <vector>
using namespace std;

enum PhoneType{XH73Q};

PhoneType etochar(const char* model)
{
	if(!strcmp(model, "XH73Q")) return XH73Q; 
}


class BuildFrame
{
public:
	//This interface is used to clone objects of specified type by customer.
	static BuildFrame* findAndClone(PhoneType type);

	//provide base build frame.
	void startBuild();
protected:

	//Every object that inherit base class must overide this method since Only it know how to clone itself.
	virtual BuildFrame* clone() = 0;
	//Every object that inherit base class must overide this method since only it know how to build rom.
	virtual void build() = 0;
	//Every object that inherit base class must overide this method to return a type that can uniquely identify self.
	virtual PhoneType returnType() = 0;

	//static function is used to operate static data.
	static void addPrototype(BuildFrame* bf)
	{
		models.push_back(bf);
	}

private:
	//This container include pointers of all models.
	static vector<BuildFrame*> models;	
};
	
	vector<BuildFrame*> BuildFrame::models; //this step allocate memory for models.
	
	BuildFrame* BuildFrame::findAndClone(PhoneType type)
	{
		for(int i = 0; i < models.size(); i++)
		{
			if(models[i]->returnType() == type)
			{
				return models[i]->clone();
			}
		}
		
		cout<<"this model don't exist in bulid list,please added it firstly."<<endl;
		exit(1);
	}

	void BuildFrame::startBuild()
	{
		cout<<"1.Init enviroment"<<endl;
		SinCmd cmd_initEnv("repof init -u ssh://zhaoyong0602@10.74.185.240:29420/FIH/QC/manifest -b OF6/dev/SDM636 -m 30100013_SP_BUILD.xml", "/home/zhaoyong/sp2/code/XH73Q");
		cmd_initEnv.excute();		

		cout<<"2.download code"<<endl;
		SinCmd cmd_downCode("repof sync -c -j4 --no-tags");
		cmd_downCode.excute();

		cout<<"3.create branch"<<endl;
		SinCmd cmd_gitBranchAll("repof start BASE --all");		
		cmd_gitBranchAll.excute();

		cout<<"4.cherry pick"<<endl;

		build();

		cout<<"6.replace"<<endl;

		cout<<"7.upload packages"<<endl;
	}

#endif
