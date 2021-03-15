#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
using namespace std;

long getFileSize(string filename)
{
    struct stat infos;
    if(stat(filename.c_str(), &infos) < 0)
    {
        perror("stat error");
        exit(-1);
    }

    //printf("file size: %ld\n", info.st_size);     

    return infos.st_size;
}


FILE* getslsize(FILE* pf, int linenum, int* length)
{
	FILE* stream = pf;
	char* line = NULL;
	size_t len = 0;	
	ssize_t nread;

	int i = 0;
	while ((nread = getline(&line, &len, stream)) != -1)
	{
		i++;
		if(i == linenum - 1)
		{
			*length = strlen(line);
			break;
		}
	}

	free(line);

	return stream;
}


int main(int argc, char* argv[])
{

	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
		exit(-1);
	}


	FILE* pf1 = fopen(argv[1], "r+");
	if(!pf1)
	{
		perror("open file error :");
		exit(-1);
	}

	FILE* pf2 = fopen(argv[1], "r");
	if(!pf2)
	{
		perror("open file error :");
		exit(-1);
	}
		

    int length = 0;
	int t;
	//定位文件第十行
	FILE* mpf1 = getslsize(pf1, 10, &t);
	//移动到行始
	//fseek(mpf1, SEEK_CUR, 1);

	FILE* mpf2 = getslsize(pf2, 11, &length);

	printf("10 lines = %d\n", length);
	
	//移动到行始
	//fseek(mpf2, SEEK_CUR, 1);

	//获取文件大小
	int sum = getFileSize(argv[1]);
	printf("sum = %d\n", sum);	
	char buf[1024] = {0};
	while(fgets(buf, 1024, pf2))
	{
		printf("%s",buf);
		fputs(buf, pf1);
	}


	truncate(argv[1], sum - length);

	fclose(pf1);
	fclose(pf2);
	return 0;
}
