#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <string.h>
#include <crypt.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage..\n");
		exit(1);
	}

	char* input_pass;
	struct spwd* sl;

	//关掉终端回写功能
	input_pass = getpass("PassWord:");

	sl = getspnam(argv[1]);

	char* ct;

	ct = crypt(input_pass,sl->sp_pwdp);
	
	if(!strcmp(sl->sp_pwdp, ct))
	{
		puts("ok");
	}
	else
	{
		puts("failed");
	}

	return 0;
}
