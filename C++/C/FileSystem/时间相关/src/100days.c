#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define TIMESTRSIZE 1024

int main()
{
	time_t stamp;
	struct tm* tm;
	char timestr[TIMESTRSIZE];

	stamp = time(NULL);
	tm = localtime(&stamp);
	strftime(timestr, TIMESTRSIZE, "Now:%Y-%m-%d", tm);

	puts(timestr);

	tm->tm_mday += 100;
	(void)mktime(tm);
	//此函数先会调整不合法的时间

	strftime(timestr, TIMESTRSIZE, "100 days later:%Y-%m-%d", tm);
	puts(timestr);

	//60*60*24*100	

}
