#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

#define BUFSIZE 1024


int main()
{
	int ret;

	char buf1[1024] = {1};
	char* buf = (char*)malloc(sizeof(char) * BUFSIZE);

	if(!buf)
	{
		perror("molloc error");
		exit(1);
	}

	memset(buf, 0, BUFSIZE);

	printf("buf1[2] = %d\n", buf1[2]);
	printf("buf[1023] = %d\n", buf[1023]);
	printf("buf1[1023] = %d\n", buf1[1023]);
	printf("buf1[0] = %d\n", buf1[0]);
	printf("buf1[2] = %d\n", buf1[2]);

	return 0;
}
