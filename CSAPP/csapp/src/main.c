#include <stdio.h>

#include "cpu/register.h"
#include "memory/instruction.h"

int main()
{

	reg.rax = 0x123456789ABCDEF0;

	printf("rax = 0x%lx\n", reg.rax);
	printf("eax = 0x%x\n", reg.eax);
	printf("ax = 0x%x\n", reg.ax);
	printf("ah = 0x%x\n", reg.ah);
	printf("al = 0x%x\n", reg.al);
	

	return 0;
}
