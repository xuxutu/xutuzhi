#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv)
{
	if(argc < 3)
	{
		fprintf(stderr, "Usage:%s <drc_file> <dest_file>\n", argv[0]);
		exit(0);
	}

	FILE *pfs, *pfd;

	if((pfs = fopen(argv[1], "r")) == NULL)
	{
		perror("fopen error");
		exit(1);
	}

	if((pfd = fopen(argv[2], "w")) == NULL)
	{
		perror("fopen error");
		fclose(pfs);
		exit(1);	
	}
	int n;
	char buf[1024];

	while((n = fread(buf, 1, 1024, pfs)) > 0)
		fwrite(buf, 1, n, pfd);

	fclose(pfd);
	fclose(pfs);

	return 0;
}
