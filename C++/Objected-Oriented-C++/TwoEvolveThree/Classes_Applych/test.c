#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;

int main()
{
	time_t t;
	time(&t);
	struct tm* timeinfo;
	
	timeinfo = gmtime(&t);
	char date[32] = {0};

	if(timeinfo->tm_mon < 10)
		sprintf(date, "%d0%d", timeinfo->tm_year+1900,timeinfo->tm_mon);
	else
		sprintf(date, "%d%d", timeinfo->tm_year+1900,timeinfo->tm_mon);
	puts(date);	

	return date;
}
