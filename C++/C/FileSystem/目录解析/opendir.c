#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define PAT "/etc"


int main()
{
	DIR* dir;
	struct dirent* cur;


	dir = opendir(PAT);
	if(dir == NULL)
	{
		perror("opendir error:");
		exit(-1);	
	}

	while((cur = readdir(dir)) != NULL )
	{
		puts(cur->d_name);
	}

	closedir(dir);

	return 0;
}
