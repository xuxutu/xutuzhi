#include <stdio.h>
#include <stdlib.h>
#include <glob.h>


#define PAT		"/etc/a*.conf"

#if 0
int errfunc_(char* errpath, int eerrno)
{
	puts(errpath);
	fprintf(stderr, "glob error: %s\n", strerror(ererrno));
}
#endif

//glob的第三个回调函数，用于报错，可以写为NULL

int main()
{
	glob_t globres;
	int err;	

	err = glob(PAT, 0, NULL, &globres);

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
