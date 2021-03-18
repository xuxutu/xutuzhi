#include<stdio.h>
#include<stdlib.h>

enum{A=10,B,C,D,E,F};

//10进制转换为16进制

void hex(int a)
{
	if(a==0)
	{
		return;
	}

	hex(a/16);
	//18=1*16+2;
	//1=0*16+1


	switch(a%16)
	{
		case A:
			{
				printf("%c",'A');
				break;
			}

		case B:
			{
				printf("%c",'B');
				break;
			}

		case C:
			{
				printf("%c",'C');
				break;
			}

		case D:
			{
				printf("%c",'D');
				break;
			}

		case E:
			{
				printf("%c",'E');
				break;
			}

		case F:
			{
				printf("%c",'F');
				break;
			}

		default:
			{
				printf("%d",a%16);		
			}
	}

}

int main(void)
{
	int a;
	while(1)
	{
		scanf("%d",&a);
		printf("0x");
		hex(a);
		printf("\n");
	}

	return 0;
}
