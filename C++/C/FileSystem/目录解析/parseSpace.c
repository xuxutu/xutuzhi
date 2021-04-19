#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <stdio.h>
#include <string.h>

#define PAT		" /"

#define STR "ls -l -a -s -d -a/a/a/a/"

#if 0
int errfunc_(char* errpath, int eerrno)
{
	puts(errpath);
	fprintf(stderr, "glob error: %s\n", strerror(ererrno));
}
#endif

//glob的第三个回调函数，用于报错，可以写为NULL

int main(int argc, char* argv[])
{
	char* p[1];

	char buf[32] = STR;

	p[0] = buf;
	glob_t globres;
	int err;	
	char* tok;

	
	while(tok = strsep(p, PAT))
	{
		if(!tok)
			perror("strep error:");
		err = glob(tok, GLOB_NOCHECK | GLOB_APPEND, NULL, &globres);
	}


	if(err)
	{
		printf("Error code = %d\n", err);
		exit(-1);	
	}


	for(int i = 0; i < globres.gl_pathc; i++)
		puts(globres.gl_pathv[i]);	

	globfree(&globres);

	return 0;
}
