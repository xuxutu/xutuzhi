#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define  FNAME 		"/tmp/out"
#define  BUFSIZE	1024		


int main()
{
	char buf[BUFSIZE];
	int count = 0;
	time_t stamp;
	struct tm* tm;

	FILE* pf = fopen(FNAME, "a+");
	if(!pf)
	{	
		perror("fopen error: ");	
		exit(-1);	
	}

	while(fgets(buf, BUFSIZE, pf))
	{	
		count++;	
	}

	while(1)
	{
		time(&stamp);
		tm = localtime(&stamp);
		fprintf(pf, "%-4d%d-%d-%d %d:%d:%d\n", ++count,\
			tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,\
			tm->tm_hour, tm->tm_min, tm->tm_sec);
		fflush(pf);//文件是全缓冲，遇到\n并不会刷新缓冲区		

		sleep(1);		
	}


	fclose(pf);


	return 0;
}
