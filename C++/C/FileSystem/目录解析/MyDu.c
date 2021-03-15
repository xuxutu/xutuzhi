#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>

#define PATHSIZE 1024

// /aaa/sd/d//sd/fd/f/sd/f/

static int not_loop(const char* path)
{
	char* pos;
	if(!(pos = strrchr(path, '/')))
	{
		perror("strrchr error");
		exit(-1);
	}	

	if(!strcmp(pos + 1, ".") || !strcmp(pos + 1, ".."))
		return 0;


	return 1;
}

int64_t mydu(const char* path)
{
/*
	横跨整个递归点的变量不能放在静态区
*/
	glob_t globres;
	static struct stat statres;
	int err;
	static char nextpath[PATHSIZE];
	int64_t sum;
	int i;


	if(lstat(path, &statres))
	{
		perror("lstat error!");
		exit(1);
	}

	if(!S_ISDIR(statres.st_mode))
		return statres.st_blocks;

	strncpy(nextpath, path, PATHSIZE);
	strncat(nextpath, "/*", PATHSIZE);		
	err = glob(nextpath, 0, NULL, &globres);
/*	if(err)
	{
		fprintf(stderr, "glob error code: %d\n", err);
		exit(-1);
	}
*/
	strncpy(nextpath, path, PATHSIZE);
	strncat(nextpath, "/.*", PATHSIZE);
	//追加上回解析的pathv
	err = glob(nextpath, GLOB_APPEND, NULL, &globres);
/*	if(err)
	{
		fprintf(stderr, "glob error code: %d\n", err);
		exit(-1);
	}
*/	
	sum = statres.st_blocks;

	for(i = 0; i < globres.gl_pathc; i++)
	{
		if(not_loop(globres.gl_pathv[i]))
			sum += mydu(globres.gl_pathv[i]);
	}

	globfree(&globres);
	
	return sum;
}


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s [pattern]\n", argv[0]);
		exit(-1);
	}

	int64_t size = mydu(argv[1]) / 2 / 1024 / 1024;

	printf("size = %ld GB\n", size);

	return 0;
}
