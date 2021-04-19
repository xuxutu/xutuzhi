#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	printf("Begin");

//	fflush(NULL);

	execl("/bin/date", "date", "+%s", NULL);

	perror("execl error:");
	exit(-1);

	puts("End");

	return 0;
}
