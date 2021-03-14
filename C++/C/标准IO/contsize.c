#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s file name\n",argv[0]);
		exit(1);
	}


	FILE* pf;

	if((pf = fopen(argv[1], "r")) == NULL)
	{
		perror("fopen error");
		exit(1);
	}

	int count = 0;

	while(fgetc(pf) != EOF)
	{
		count++;
	}

	printf("%s size = %d\n", argv[0], count);

	fclose(pf);

	return 0;
}
