#include <iostream>
#include "buildlists.h"

using namespace std;


int main(int argc, const char* argv[])
{

	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s [model]\n", argv[0]);
		exit(1);
	}

	(BuildFrame::findAndClone(etochar(argv[1])))->startBuild();


	return 0;
}
