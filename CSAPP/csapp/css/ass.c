#include "cpu.h"
#include <stdio.h>

int main()
{

	cpu_t cpu;
	cpu.rax = 0x123456789ABCDEF0;

	printf("rax = 0x%lx\n", cpu.rax);
	printf("eax = 0x%x\n", cpu.eax);
	printf("ax = 0x%x\n", cpu.ax);
	printf("ah = 0x%x\n", cpu.ah);
	printf("al = 0x%x\n", cpu.al);
	

	return 0;
}
