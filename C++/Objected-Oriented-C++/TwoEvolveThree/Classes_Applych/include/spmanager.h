#ifndef __SPMANAGER__
#define __SPMANAGER__
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "csvloader.h"
#include <algorithm>


using namespace std;


extern char* getDate(char *date);

struct patch_files
{
	string project;
	map<int,string> pfiles;
};

struct Case
{
	string cve;
	string version;
	string patch_level;
	string android_id;
	vector<patch_files> patchfiles;
};


struct ticket
{
	int tid;
	string cve;	
	string version;
	string patch_level;
	string android_id;
	string project;
	int pnum;
	string patchfile;
	string inbranch;
	bool isinque;
	bool isApply;		
};


class SpManager
{
public:
	SpManager(){}

	SpManager(string spfile, string version);

	void showticket()const;	
	void showcase()const;
	void sorticket();
	vector<ticket>& getTickets(){ return sortick; }
	vector<string> getprojects()const{ return prolist; }
private:
	vector<Case> ptick;
	vector<ticket> tks;
	vector<string> prolist;
	vector<ticket> sortick;	
};

#endif
