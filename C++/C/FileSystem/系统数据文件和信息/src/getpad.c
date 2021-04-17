#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char* argv[])
{
	struct passwd* pwdline;



	pwdline = getpwuid(atoi(argv[1]));


	puts(pwdline->pw_passwd);





	return 0;
}
