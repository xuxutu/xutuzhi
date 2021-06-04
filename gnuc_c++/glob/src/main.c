#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myglob.h"

#define DELIMS " "
#define STR "grep find cat touch"



int main()
{
	myglob_t globres;




	char* tok;
	int i = 0;
	char buf[1024] = STR;
	char* p = buf;	
	


/*	while(1)
	{
		tok = strsep(&p, DELIMS);
		if(tok == NULL)
			break;
		if(!tok[0])
			continue;
		//puts(tok);

		myglob(tok, GLOB_NOCHECK | GLOB_APPEND * i, NULL, &globres);
		
		i = 1;
	}	
*/
	myglob(STR, GLOB_NOCHECK, NULL, &globres);
/*
	for(int i = 0; i < globres.gl_pathc; i++)
		puts(globres.gl_pathv[i]);

	myglobfree(&globres);
*/
	return 0;
}
