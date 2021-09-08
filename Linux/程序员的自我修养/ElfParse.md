# 1. 典型代码

![image-20210816132605278](ElfParse.assets/image-20210816132605278.png)	



# 2. ELF头定义

## 2.1 ELF文件定义类型大小

```
Elf64_Half   =  2
Elf64_Word  = 4
Elf64_Addr  = 8
Elf64_Off = 8
Elf64_Xword = 8
```







![image-20210907141358583](ElfParse.assets/image-20210907141358583.png)	



## 2.2 可重定位文件头信息

![image-20210907141939176](ElfParse.assets/image-20210907141939176.png)	

## 2.3 可重定位文件头解析

![image-20210907141446025](ElfParse.assets/image-20210907141446025.png)

# 3. 段表定义

![image-20210907142230785](ElfParse.assets/image-20210907142230785.png)	

## 3.1 可重定位文件段表信息

![image-20210907142103579](ElfParse.assets/image-20210907142103579.png)	

## 3.2 可重定位文件段表解析

![image-20210907155917276](ElfParse.assets/image-20210907155917276.png)	

# 4.重定位表



![](ElfParse.assets/image-20210907171821521.png)	



SHT_RELA = 4,重定位表。

sh_info = 1,表示对.text重定位



# 完整elf图解

![image-20210907171204485](ElfParse.assets/image-20210907171204485.png)

