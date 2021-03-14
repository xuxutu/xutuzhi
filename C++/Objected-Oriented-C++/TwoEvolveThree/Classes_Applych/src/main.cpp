#include <getopt.h>
#include <stdio.h>
#include "applychlist.h"
#include "applychframe.h"


int main(int argc, char* argv[])
{
	
	//SpManager -m/--model QE50Q 
	//this option will apply all patches

	//SpManager --no-apply "platform/frameworks/base" -m/--models QE50Q	
	//this option won't apply some patches belong to this project "platform/frameworks/base"		
		
	//SpManager showinfo
	//output project name at which some patches be patched in that month,pathfile in this project and the number of patch file.
 
	//SpManager -c/--check + version, 
	//output the infomation releted to the patches of corresponding verson and modification information of the patch file.

	//SpManager -c +version 
	//output all tickets of corresponding verson

	string date;

	int c;
    int digit_optind = 0;

	string version,model;

    while (1) 
	{
    	int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = 
		{
        	{"add",      required_argument, 0, 'a'},
            {"appench",  no_argument,       0, 	0 },
            {"delete",   required_argument, 0,  1 },
            {"date",	 required_argument, 0, 'd'},
            {"verbose",  no_argument,       0,  0 },
           	{"create",   required_argument, 0, 'c'},
            {"model",    required_argument, 0, 'm'},
            {"version",  required_argument, 0, 'v'},
			{"no-apply", required_argument,	0, 'n'},
			{"showtick", no_argument,		0, 'k'},
			{"showcase", no_argument,		0, 'e'},
            {0,          0,                 0,  0 }
        };

       	c = getopt_long(argc, argv, "bc:dm:n:v:012",
                        long_options, &option_index);
       	if (c == -1)
        break;
			
        switch (c) 
		{
        	case 0:
            {
				printf("option %s", long_options[option_index].name);
                if (optarg)
                {
					printf(" with arg %s", optarg);
				}
                printf("\n");
                break;
			}
			//三者只能有一种存在
           	case '0':
            case '1':
            case '2':
            {
				if (digit_optind != 0 && digit_optind != this_option_optind)
                { 
					printf("digits occur in two different argv-elements.\n");
				}
                digit_optind = this_option_optind;
                printf("option %c\n", c);
                break;
			}
       		case 'a':
			{		
            	printf("option a\n");
                break;
			}
           	case 'b':
            {   printf("option b\n");
                break;
			}
			case 'd':
			{
				date = string(optarg);
			
				//printf("optind = %d, argc = %d", optind, argc);
	
				//Fistrly selected model.
				if(model[0] && date[0])
				{
					if(optind == argc)
					{	
						(Applych::findAndClone(strtoenu(optarg), date))->startApply();
						break;
					}
					else
						goto out;
				}
				break;
			}
           	case 'm':
            {   
				model = string(optarg);
				//printf("option m with value '%s'\n", optarg);
				if(optind == argc)
				{
					
					(Applych::findAndClone(strtoenu(optarg), date))->startApply();
				}
			
                break;
			}
			case 'v':
			{
				version = string(optarg);
				printf("option m with value '%s'\n", optarg);
				if(model[0] && version[0])
				{
					//SpManager app(version);
				}
				else
				{
					if(optind == argc) cout<<"please enter model infomation!"<<endl;
				}
				break;
			}
            case 'n':
			{
               	printf("option n with value '%s'\n", optarg);
                break;
			}
			case 'k':
			{
				if(version[0] == 0 && optind != argc)
					goto out;
				else
				{
					//SpManager app(version);
					//app.showticket();
				}	
				break;
			}
			case 'e':
			{
				if(version[0] == 0 && optind != argc)
					goto out;
				else
				{
					//SpManager app(version);
					//app.showcase();
				}

				break;
			}
            case '?':
			{
             	break;
			}
            default:
			{
                printf("?? getopt returned character code 0%o ??\n", c);
            }
    	}

	} 
		
out:	
	if (optind < argc)
	{
    	printf("non-option ARGV-elements: ");
        while(optind < argc)
        {
			printf("%s ", argv[optind++]);
		}
               	
		printf("\n");
	}


	exit(EXIT_SUCCESS);
}
