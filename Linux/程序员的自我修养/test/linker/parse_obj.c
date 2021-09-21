#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void parser_elf_header(Elf64_Ehdr* ehdr)
{
	printf("段表偏移:0x%lX\n", ehdr->e_shoff);
	printf("段表字符串描述符在段表中的索引:0x%x\n", ehdr->e_shstrndx);
	printf("段表描述符的数量:0x%x\n", ehdr->e_shnum);
}

void parser_elf_sectb(Elf64_Shdr* sectb)
{
	printf("段表字符串表在文件中的偏移:0x%lX\n", sectb->sh_offset);
	printf("段表字符串表的长度:0x%lX\n", sectb->sh_size);
}


void get_secname(Elf64_Shdr* sectb, char* buf, int c)
{
	printf("所有段名:\n");
	for(int i = 0; i < c; i++)
	{	
		puts(buf + (sectb + i)->sh_name);
	}

}

int get_sec_index(Elf64_Shdr* sectb, char* buf, int c, char* sec_name)
{
	int flag = -1;
	for(int i = 0; i < c; i++)
	{
		if(!strcmp(buf + (sectb + i)->sh_name, sec_name))
		{
			flag = i;
		}
	}

	return flag;
}

void parser_elf_symtb(Elf64_Sym* symtb, char* buf, int c)
{
	for(int i = 0; i < c; i++)
	{
		puts(buf + (symtb + i)->st_name);
		printf("0x%lx\n", (symtb + i)->st_name);
	}
}

int get_rel_index(Elf64_Shdr* sectb, char* buf, int c, int s)
{

	int flag = -1;
	for(int i = 0; i < c; i++)
	{
		if((sectb + i)->sh_type == s)
		{
			printf("index = 0x%lX\n", i);
			puts((sectb + i)->sh_name + buf);
			flag = i;
		}
	}

	return flag;
}

void parser_elf_reltb(Elf64_Rel* reltb, char* buf, int c)
{
	for(int i = 0; i < c; i++)
	{
		printf("重定位入口的偏移:0x%lX\n", reltb[i].r_offset);
		printf("重定位入口的类型和符号:0x%lX\n", reltb[i].r_info);
	}
}

int main(int argc, const char** argv)
{
	char header_buf[sizeof(Elf64_Ehdr)];


	if(argc < 2)
	{
		fprintf(stderr, "usage: %s <obj file>\n", argv[0]);
		exit(1);
	}	

	int fd = open(argv[1], O_RDONLY);
	if(fd < 0)
	{
		perror("open error");
		exit(1);
	}

	if(read(fd, header_buf, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr))
	{
		perror("read error");
		exit(1);
	}	

	Elf64_Ehdr* head = (Elf64_Ehdr*)header_buf;	
	//获取头部信息
	parser_elf_header(head);

	//定位到段表
	int sectb_size = sizeof(Elf64_Shdr) * head->e_shnum;	
	char sectb_buf[sectb_size];
	lseek(fd, head->e_shoff, SEEK_SET);
	if(read(fd, sectb_buf, sectb_size) != sectb_size)
	{
		perror("read error");
		exit(1);
	}	
	
	Elf64_Shdr* sectb = (Elf64_Shdr*)sectb_buf + head->e_shstrndx;
	parser_elf_sectb(sectb);
	

	//获取所有段名
	int secstr_size = sectb->sh_size;
	char secstr_buf[secstr_size];
	lseek(fd, sectb->sh_offset, SEEK_SET);
	
	if(read(fd, secstr_buf, secstr_size) != secstr_size)
	{
		perror("read error");
		exit(1);
	}

	get_secname((Elf64_Shdr*)sectb_buf, secstr_buf, head->e_shnum);
	

	//获取字符串表的索引
	int char_str_index = get_sec_index((Elf64_Shdr*)sectb_buf, secstr_buf, head->e_shnum, ".strtab");
	int chartb_size = ((Elf64_Shdr*)sectb_buf + char_str_index)->sh_size;
	char chartb_buf[chartb_size];

	lseek(fd, ((Elf64_Shdr*)sectb_buf + char_str_index)->sh_offset, SEEK_SET);
	if(read(fd, chartb_buf, chartb_size) != chartb_size)
	{
		perror("read error");
		exit(1);
	}
	

	//获取符号表的索引
	int sym_str_index = get_sec_index((Elf64_Shdr*)sectb_buf, secstr_buf, head->e_shnum, ".symtab");

	printf("符号表所在索引:%d\n", sym_str_index);

	//获取符号表信息
	int symtb_size = ((Elf64_Shdr*)sectb_buf + sym_str_index)->sh_size;
	char symtb_buf[symtb_size];	

	lseek(fd, ((Elf64_Shdr*)sectb_buf + sym_str_index)->sh_offset, SEEK_SET);
	if(read(fd, symtb_buf, symtb_size) != symtb_size)
	{
		perror("read error");
		exit(1);
	}

	Elf64_Sym* symtb = (Elf64_Sym*)symtb_buf;

	printf("符号表大小:%d, 每项大小:%ld | %d\n", symtb_size, sizeof(Elf64_Sym), ((Elf64_Shdr*)sectb_buf + sym_str_index)->sh_entsize);	
	parser_elf_symtb(symtb, chartb_buf, symtb_size / sizeof(Elf64_Sym));

	//获取可从定位表
	//获取可重定位表的索引
	puts("reltb");


	int reltb_index = 2;
	int reltb_size = ((Elf64_Shdr*)sectb_buf + reltb_index)->sh_size;
	char reltb_buf[reltb_size];
	lseek(fd, ((Elf64_Shdr*)sectb_buf + reltb_index)->sh_offset, SEEK_SET);	
	if(read(fd, reltb_buf, reltb_size) != reltb_size)
	{
		perror("read error");
		exit(1);
	}
	
	Elf64_Rel* reltb = (Elf64_Rel*)reltb_buf;
	parser_elf_reltb(reltb, symtb_buf, reltb_size / sizeof(Elf64_Rel));
	printf("重定位表项数:%d %d\n", ((Elf64_Shdr*)sectb_buf + reltb_index)->sh_entsize, reltb_size / sizeof(Elf64_Rel));	
/*
	while((reltb_index = get_rel_index((Elf64_Shdr*)sectb_buf, secstr_buf, head->e_shnum, SHT_RELA)) != -1)
	{
		printf("可重定位表的索引:%d\n", reltb_index);
		int reltb_size = ((Elf64_Shdr*)sectb_buf + reltb_index)->sh_size;
		char reltb_buf[reltb_size];

		lseek(fd, ((Elf64_Shdr*)sectb_buf + reltb_index)->sh_offset, SEEK_SET);	
		if(read(fd, reltb_buf, reltb_size) != reltb_size)
		{
			perror("read error");
			exit(1);
		}
		puts("-------------------------");
		puts(((Elf64_Shdr*)sectb_buf + reltb_index)->sh_name + symtb_buf);
		Elf64_Rel* reltb = (Elf64_Rel*)reltb_buf;
		parser_elf_reltb(reltb, symtb_buf, reltb_size / sizeof(Elf64_Rel));
		printf("重定位表项数:%d %d\n", ((Elf64_Shdr*)sectb_buf + reltb_index)->sh_entsize, reltb_size / sizeof(Elf64_Rel));
	}
*/
	return 0;
}
