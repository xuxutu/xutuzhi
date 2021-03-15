#include "patchUrlmanager.h"
#include<iostream>
using namespace std;

int main(int argc,const char* argv[])
{

	for(int i=1;i<argc;i++)
	{
		PatchUrlManager pum(argv[i]);	
		pum.downpatch();
	}
	
	pthread_exit(NULL);
}
