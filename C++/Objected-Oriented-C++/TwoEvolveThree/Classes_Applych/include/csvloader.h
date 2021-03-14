#ifndef __CSVLOADER__
#define __CSVLOADER__
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <vector>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

class Csvloader
{
public:
	Csvloader(string filename);
	
	
	//return table header
	vector<vector<string>> getHead(void)const 
	{
		return msv;
	}
	
	int get_index(string value);

	int getRow()const{ return row;}
	int getClu()const{ return column;}

private:
	string m_filename;
	int row;
	int column;	
	vector<vector<string>> msv;
};

extern long getFileSize(string filename);
#endif
