#ifndef __PATCHURLMANAGER__
#define	__PATCHURLMANAGER__
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

using namespace std;

struct Cmd
{
	string cmd1;  	
	string cmd2;
	string cmd3;		
};

class PatchUrlManager
{
public:
	PatchUrlManager(const char* cr);//初始化cr
	void showCr();
	const string getPath(string str);
	const string getCommit(string str);
	void showUrl();
	static void* __downpatch(void* purl);
	void downpatch();
private:
	string m_cr;
	string filename;
	vector<string>m_purl;	
	Cmd cmd;	
};



#endif
