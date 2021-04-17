# 命令行解析

## get_opt

```
       #include <unistd.h>

       int getopt(int argc, char * const argv[],
                  const char *optstring);
    
       extern char *optarg;
       extern int optind, opterr, optopt;

```

### optstring

创建一个特殊名字的文件 -a

```
touch ./-a
touch -- -a
```

所以说**optstring** 不包含 -，那个减号只时提示你：

**如果当前选项是一个-，表示一个option**

**如果当前选项是两个-,表示一个long option**

**如果是两个-后什么没有，表示选项结束**

### optarg

指向选项后的串

### optind

类似文件位置指针

**-表示非选项传参**，返回1

```
 c = getopt(argc, argv, "-H:MSy:md");
```



### 程序设计

```
-y : year

-m : month

-d : day

-H : hour

-M : minute

-S : second
```

