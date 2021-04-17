#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define TIMESTRSIZE 1024

#define FMTSTRSIZE 1024


int main(int argc, char* argv[])
{
	int c;
	char fmtstr[FMTSTRSIZE] = {0};	
	FILE* pf = stdout;


	time_t stamp;
	struct tm* tm;
	char timestr[TIMESTRSIZE];

	stamp = time(NULL);
	tm = localtime(&stamp);


	while(1)
	{
		c = getopt(argc, argv, "-H:MSy:md");
		if(c < 0)
			break;	

		switch(c)
		{
			case 1:
			{
				//非选项传参

				if(pf == stdout)
				{
					pf = fopen(argv[optind-1], "w");
					if(pf == NULL)
					{
						perror("fopen:");
						pf = stdout;
					}
				}
				break;
			}

			case 'H':
			{	
				if(strcmp(optarg, "12") == 0)
					strncat(fmtstr, "%I(%P) ", FMTSTRSIZE);			
				else if(strcmp(optarg, "24") == 0)
					strncat(fmtstr, "%H ", FMTSTRSIZE);		
				else
					fprintf(stderr, "Invalid argument of H\n");
				break;
			}
			
			case 'M':
			{
				strncat(fmtstr, "%M ", FMTSTRSIZE);
				break;
			}

			case 'S':
			{
				strncat(fmtstr, "%S ", FMTSTRSIZE);
				break;
			}

			case 'y':
			{
				if(strcmp(optarg, "2") == 0)
					strncat(fmtstr, "%y ", FMTSTRSIZE);			
				else if(strcmp(optarg, "4") == 0)
					strncat(fmtstr, "%Y ", FMTSTRSIZE);		
				else
					fprintf(stderr, "Invalid argument of Y\n");
				break;
				break;
			}

			case 'm':
			{
				strncat(fmtstr, "%m ", FMTSTRSIZE);
				break;
			}
			
			case 'd':
			{
				strncat(fmtstr, "%d ", FMTSTRSIZE);
				break;
			}

			default:
				break;		
		}
	}
	strncat(fmtstr, "\n", FMTSTRSIZE);
	strftime(timestr, TIMESTRSIZE, fmtstr, tm);
	fputs(timestr, pf);


	if(pf != stdout)
		fclose(pf);	
#if 0

	tm->tm_mday += 100;
	(void)mktime(tm);
	//此函数先会调整不合法的时间

	strftime(timestr, TIMESTRSIZE, "100 days later:%Y-%m-%d", tm);
	puts(timestr);

	//60*60*24*100	
#endif
	return 0;
}
