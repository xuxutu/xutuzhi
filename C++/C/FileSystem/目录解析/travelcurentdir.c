#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
int main()
{

	struct dirent **ptr;

	int num = scandir(getcwd(NULL, 0), &ptr, NULL, alphasort);

	for(int i =0; i < num; i++)
		puts(ptr[i]->d_name);

	return 0;
}


