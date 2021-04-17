#include <stdio.h>
#include <string.h>
#include "setenv.h"

extern char** environ;

int main()
{

	setenv("ZHAOYONG", "0602", 1);
	
	puts("hehe");
	
	for(int i = 0; environ[i] != NULL; i++)
		puts(environ[i]);


	setenv("ZHAOYONG", "06023453545", 1);
	for(int i = 0; environ[i] != NULL; i++)
		puts(environ[i]);
}
