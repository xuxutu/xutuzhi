#include <stdio.h>
#include <elf.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#if 0
typedef struct
{
  unsigned char e_ident[EI_NIDENT]; /* Magic number and other info */
  Elf64_Half    e_type;         /* Object file type */
  Elf64_Half    e_machine;      /* Architecture */
  Elf64_Word    e_version;      /* Object file version */
  Elf64_Addr    e_entry;        /* Entry point virtual address */
  Elf64_Off e_phoff;        /* Program header table file offset */
  Elf64_Off e_shoff;        /* Section header table file offset */
  Elf64_Word    e_flags;        /* Processor-specific flags */
  Elf64_Half    e_ehsize;       /* ELF header size in bytes */
  Elf64_Half    e_phentsize;        /* Program header table entry size */
  Elf64_Half    e_phnum;        /* Program header table entry count */
  Elf64_Half    e_shentsize;        /* Section header table entry size */
  Elf64_Half    e_shnum;        /* Section header table entry count */
  Elf64_Half    e_shstrndx;     /* Section header string table index */
} Elf64_Ehdr;
#endif



int main()
{
	Elf64_Ehdr header;

	printf("Elf64_Half = %d\n", sizeof(header.e_type));
	printf("Elf64_Word = %d\n", sizeof(header.e_version));
	printf("Elf64_Addr = %d\n", sizeof(header.e_entry));
	printf("Elf64_Off = %d\n", sizeof(header.e_phoff));
	printf("Elf64_Xword = %d\n", sizeof(Elf64_Xword));
	printf("size = %d\n", sizeof(header));


	int fd = open("ss.o", O_RDONLY);
	if(fd < 0)
	{
		perror("open error: ");
		exit(1);
	}

	if(read(fd, &header, sizeof(header)) != sizeof(header))
	{
		perror("read error: ");
		exit(1);	
	}

	printf("Magic number and other info = %x ", header.e_ident[0]);
	printf("%c ", header.e_ident[1]);
	printf("%c ", header.e_ident[2]);
	printf("%c ", header.e_ident[3]);
	printf("0%d ", header.e_ident[4]);
	printf("0%d ", header.e_ident[5]);
	printf("0%d ", header.e_ident[6]);
	printf("0%d ", header.e_ident[7]);
	printf("0%d ", header.e_ident[8]);
	printf("0%d ", header.e_ident[9]);
	printf("0%d ", header.e_ident[10]);
	printf("0%d ", header.e_ident[11]);
	printf("0%d ", header.e_ident[12]);
	printf("0%d ", header.e_ident[13]);
	printf("0%d ", header.e_ident[14]);
	printf("0%d\n", header.e_ident[15]);
	
	if(header.e_ident[4] == 2)
		printf("machine width = 64\n");
	if(header.e_ident[5] == 1)
		printf("Iittle endian\n");

	printf("---------------------------------------------------------------------------\n");

	printf("Object file type = %d\n", header.e_type);
	printf("Architecture = %d\n", header.e_machine);
	printf("Object file version = %d\n", header.e_version);
	printf("Entry point virtual address = %ld\n", header.e_entry);
	printf("Program header table file offset = %ld\n", header.e_phoff);
	printf("Section header table file offset = %ld\n", header.e_shoff);
	printf("Processor-specific flags = %d\n", header.e_flags);
	printf("ELF header size in bytes = %d\n", header.e_ehsize);
	printf("Program header table entry size = %d\n", header.e_phentsize);
	printf("Program header table entry count = %d\n", header.e_phnum);
	printf("Section header table entry size = %d\n", header.e_shentsize);
	printf("Section header table entry count = %d\n", header.e_shnum);
	printf("Section header string table index = %d\n", header.e_shstrndx);

	if(header.e_type == ET_REL)
		puts("可重定位文件");
	else if(header.e_type == ET_EXEC)	
		puts("可执行文件");
	else if(header.e_type == ET_DYN)
		puts("共享目标文件");
	
	switch(header.e_machine)
	{
		case EM_M32:
		{
			puts("AT&T");
			break;
		}
		case EM_SPARC:
			puts("SPARC");
			break;
		case EM_386:
			puts("Intel x86");
			break;
		case EM_68K:
			puts("Motorola 68000");
			break;
		case EM_88K:
			puts("Motorola 88000");
			break;
		case EM_860:
			puts("Intel 80860");
			break;
		case EM_X86_64:
			puts("Advanced Micro Devices X86-64");
			break;	
		default:
			puts("read error");
			break;
	}

	int size = sizeof(Elf64_Shdr) * header.e_shnum;
	printf("size = %d\n", size);
	printf("sizeof(Elf64_Shdr) = %d\n", sizeof(Elf64_Shdr));

		
	off_t res = lseek(fd, header.e_shoff - header.e_ehsize, SEEK_CUR);
	if(res < 0)
	{
		perror("lseek error: ");
		exit(1);
	}

	char Sec_table[size];
	
	if(read(fd, &Sec_table, size) < 0)	
	{
		perror("read error: ");
		exit(1);
	}

#if 0
 398 typedef struct
 399 {
 400   Elf64_Word    sh_name;        /* Section name (string tbl index) */
 401   Elf64_Word    sh_type;        /* Section type */
 402   Elf64_Xword   sh_flags;       /* Section flags */
 403   Elf64_Addr    sh_addr;        /* Section virtual addr at execution */
 404   Elf64_Off sh_offset;      /* Section file offset */
 405   Elf64_Xword   sh_size;        /* Section size in bytes */
 406   Elf64_Word    sh_link;        /* Link to another section */
 407   Elf64_Word    sh_info;        /* Additional section information */
 408   Elf64_Xword   sh_addralign;       /* Section alignment */
 409   Elf64_Xword   sh_entsize;     /* Entry size if section holds table */
 410 } Elf64_Shdr;
#endif

puts("Section 0:");
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 0)->sh_name);
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 0)->sh_type);
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 0)->sh_flags);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 0)->sh_addr);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 0)->sh_offset);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 0)->sh_size);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 0)->sh_link);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 0)->sh_info);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 0)->sh_addralign);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 0)->sh_entsize);
puts("Section 1:");
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 1)->sh_name);
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 1)->sh_type);
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 1)->sh_flags);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 1)->sh_addr);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 1)->sh_offset);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 1)->sh_size);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 1)->sh_link);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 1)->sh_info);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 1)->sh_addralign);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 1)->sh_entsize);
puts("Section 12:");
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 12)->sh_name);
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 12)->sh_type);
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 12)->sh_flags);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 12)->sh_addr);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 12)->sh_offset);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 12)->sh_size);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 12)->sh_link);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 12)->sh_info);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 12)->sh_addralign);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 12)->sh_entsize);
puts("Section 2:");
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 2)->sh_name);
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 2)->sh_type);
	printf("%d\n", ((Elf64_Shdr *)Sec_table + 2)->sh_flags);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 2)->sh_addr);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 2)->sh_offset);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 2)->sh_size);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 2)->sh_link);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 2)->sh_info);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 2)->sh_addralign);
	printf("%ld\n", ((Elf64_Shdr *)Sec_table + 2)->sh_entsize);



	printf("%s\n", (char*)((Elf64_Shdr *)Sec_table + 12) + ((Elf64_Shdr *)Sec_table + 1)->sh_name);


printf("获取字符串表:\n");

int char_table_off = ((Elf64_Shdr *)Sec_table + 12)->sh_offset;
int char_table_size = ((Elf64_Shdr *)Sec_table + 12)->sh_size;

printf("char_table_off = %d\n", char_table_off);
printf("char_table_size = %d\n", char_table_size);

char buf[char_table_size];

//获取符号表偏移
     off_t res1 = lseek(fd, char_table_off, SEEK_SET);
     if(res1 < 0)
     {
         perror("lseek error: ");
         exit(1);
     }

    if(read(fd, buf, char_table_size) < 0)
        {
            perror("read error: ");
            exit(1);
        }
	
	for(int i = 0; i < char_table_size; i++)
	{	
		if(i == 0)	
			printf("%s\n", buf + i);
		if(buf[i] == 0)
		{	
			i++;
			if(buf[i])
			printf("%s\n", buf + i);
		}	
		
	}
	puts("");

int index = ((Elf64_Shdr *)Sec_table + 12)->sh_name;

printf("name = %c\n", buf[index]);
printf("name = %c\n", buf[index + 1]);
printf("name = %c\n", buf[index + 2]);
printf("name = %c\n", buf[index + 3]);

//	for(int i = 0; i < char_table_size; i++)
//	{
//		printf("%c ", buf[i]);
//	}

printf("获取段表字符串表:\n");
	

	return 0;
}
