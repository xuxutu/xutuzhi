#include <stdio.h>

extern char __executable_start[];
extern char etext[], _etext[], __etext[];
extern char edata[], _edata[];
extern char end[], _end[];

int main()
{
	printf("程序起始地址:%X\n", __executable_start);
	printf("代码末尾地址:%X\n", etext);
	printf("数据段结束地址:%X\n", edata);
	printf("程序结束地址:%X\n", end);

	return 0;
}
