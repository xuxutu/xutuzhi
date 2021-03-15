#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFER_LEN 1024

void getbl()
{
	//创建缓存
	char buffer[BUFFER_LEN];	

	//打开文件
	int fdin = open("contents.xml", O_RDONLY);
	if(fdin	< 0)
	{
		fprintf(stderr, "open error: %s\n", strerror(errno));
	}

	//读取文件
	ssize_t size;
	while((size = read(fdin, buffer, BUFFER_LEN)) > 0)
	{
		char* p1;
		char* p2;
		if(p1 = strstr(buffer, "<product_name>"))
		{
			p1 = p1 + strlen("<product_name>");
			if(p2 = strstr(buffer, "</product_name>"))
			{
				while(p1 != p2)
				{
					putchar(*p1);
					p1++;			
				}
				puts("");
			}
		}
	}

	if(size < 0)
	{
		fprintf(stderr, "read error: %s\n", strerror(errno));
		exit(0);
	}

}

int main(void)
{
	
	getbl();


	return 0;
}
