# 跟我一起写 Makefile

## 通配符

如果我们想定义一系列比较类似的文件，我们很自然地就想起使用通配符。make 支持 三各通配符：“*”，“?”和“[...]”。

如果是“~/test”，这就表示当前用户的$HOME 目录 下的 test 目录。而“~hchen/test”则表示用户 hchen 的宿主目录下的 test 目录。

通配符代替了你一系列的文件，如“*.c”表示所以后缀为 c 的文件。一个需要我们注 意的是，如果我们的文件名中有通配符，如：“*”，那么可以用转义字符“\”，如“\*” 来表示真实的“*”字符，而不是任意长度的字符串。



案例一

```
clean:
rm -f *.o
上面这个例子我不不多说了，这是操作系统 Shell 所支持的通配符。这是在命令中的通
配符。 
```

案例二

```
print: *.c
lpr -p $?
touch print

上面这个例子说明了通配符也可以在我们的规则中，目标 print 依赖于所有的[.c]文
件。其中的“$?”是一个自动化变量，我会在后面给你讲述。
```

案例三

```
objects = *.o
上面这个例子，表示了，通符同样可以用在变量中。并不是说[*.o]会展开，不！objects
的值就是“*.o”。Makefile 中的变量其实就是 C/C++中的宏。如果你要让通配符在变量中
展开，也就是让 objects 的值是所有[.o]的文件名的集合，那么，你可以这样： 
objects := $(wildcard *.o)

这种用法由关键字“wildcard”指出，关于 Makefile 的关键字，我们将在后面讨论。 
```



## 文件搜寻

```
VPATH = src:../headers 
```

 上面的的定义指定两个目录，“src”和“../headers”，make 会按照这个顺序进行搜 索。目录由“冒号”分隔。（当然，当前目录永远是最高优先搜索的地方）



另一个设置文件搜索路径的方法是使用 make 的“vpath”关键字（注意，它是全小写 的），这不是变量，这是一个 make 的关键字，这和上面提到的那个 VPATH 变量很类似，但是 它更为灵活。它可以指定不同的文件在不同的搜索目录中。这是一个很灵活的功能。它的使 用方法有三种：

```
1、vpath <pattern> <directories>
为符合模式<pattern>的文件指定搜索目录<directories>。
```

```
2、vpath <pattern>
清除符合模式<pattern>的文件的搜索目录。
```

```
3、vpath
清除所有已被设置好了的文件搜索目录。 
```

vapth 使用方法中的需要包含“%”字符。“%”的意思是匹配零或若干字符， 例如，“%.h”表示所有以“.h”结尾的文件。指定了要搜索的文件集，而 则指定了的文件集的搜索的目录。例如： 

```
vpath %.h ../headers 
```

该语句表示，要求 make 在“../headers”目录下搜索所有以“.h”结尾的文件。（如果 某文件在当前目录没有找到的话） 

我们可以连续地使用 vpath 语句，以指定不同搜索策略。如果连续的 vpath 语句中出现 了相同的，或是被重复了的，那么，make 会按照 vpath 语句的先后顺 序来执行搜索。如：

```
vpath %.c foo
vpath % blish
vpath %.c bar 
```

其表示“.c”结尾的文件，先在“foo”目录，然后是“blish”，最后是“bar”目录。

```
vpath %.c foo:bar
vpath % blish 
```



## 伪目标 

当然，为了避免和文件重名的这种情况，我们可以使用一个特殊的标记“.PHONY”来显 示地指明一个目标是“伪目标”，向 make 说明，不管是否有这个文件，这个目标就是“伪 目标”。 

```
.PHONY : clean 
```

只要有这个声明，不管是否有“clean”文件，要运行“clean”这个目标，只有“make clean”这样。于是整个过程可以这样写：

```
.PHONY: clean
clean:
rm *.o temp
```

伪目标一般没有依赖的文件。但是，我们也可以为伪目标指定所依赖的文件。伪目标同 样可以作为“默认目标”，只要将其放在第一个。一个示例就是，如果你的 Makefile 需要 一口气生成若干个可执行文件，但你只想简单地敲一个 make 完事，并且，所有的目标文件 都写在一个 Makefile 中，那么你可以使用“伪目标”这个特性： 

```
all : prog1 prog2 prog3
.PHONY : all

prog1 : prog1.o utils.o
cc -o prog1 prog1.o utils.o

prog2 : prog2.o
cc -o prog2 prog2.o

prog3 : prog3.o sort.o utils.o
cc -o prog3 prog3.o sort.o utils.o 
```



随便提一句，从上面的例子我们可以看出，目标也可以成为依赖。所以，伪目标同样也 可成为依赖。看下面的例子：

```
.PHONY: cleanall cleanobj cleandiff

cleanall : cleanobj cleandiff
rm program

cleanobj :
rm *.o

cleandiff :
rm *.diff 
```

“make clean”将清除所有要被清除的文件。“cleanobj”和“cleandiff”这两个伪 目标有点像“子程序”的意思。我们可以输入“make cleanall”和“make cleanobj”和 “make cleandiff”命令来达到清除不同种类文件的目的。 



## 多目标

Makefile 的规则中的目标可以不止一个，其支持多目标，有可能我们的多个目标同时依赖 于一个文件，并且其生成的命令大体类似。于是我们就能把其合并起来。当然，多个目标的 生成规则的执行命令是同一个，这可能会可我们带来麻烦，不过好在我们的可以使用一个自 动化变量“$@”（关于自动化变量，将在后面讲述），这个变量表示着目前规则中所有的目 标的集合，这样说可能很抽象，还是看一个例子吧。 

```
bigoutput littleoutput : text.g
generate text.g -$(subst output,,$@) > $@

上述规则等价于：

bigoutput : text.g
generate text.g -big > bigoutput
littleoutput : text.g
generate text.g -little > littleoutput
```

其中，-$(subst output,,$@)中的“$”表示执行一个 Makefile 的函数，函数名为 subst， 后面的为参数。关于函数，将在后面讲述。这里的这个函数是截取字符串的意思，“$@”表 示目标的集合，就像一个数组，“$@”依次取出目标，并执于命令。



## 静态模式 

```
objects = foo.o bar.o

all: $(objects)

$(objects): %.o: %.c
$(CC) -c $(CFLAGS) $< -o $@
```

上面的例子中，指明了我们的目标从$object 中获取，“%.o”表明要所有以“.o”结 尾的目标，也就是“foo.o bar.o”，也就是变量$object 集合的模式，而依赖模式“%.c” 则取模式“%.o”的“%”，也就是“foo bar”，并为其加下“.c”的后缀，于是，我们的 依赖目标就是“foo.c bar.c”。而命令中的“$<”和“$@”则是自动化变量，“**$<”表示 所有的依赖目标集**（也就是“foo.c bar.c”），“**$@”表示目标集**（也就是“foo.o bar.o”）。 于是，上面的规则展开后等价于下面的规则：

```

foo.o : foo.c
$(CC) -c $(CFLAGS) foo.c -o foo.o
bar.o : bar.c
$(CC) -c $(CFLAGS) bar.c -o bar.o
```



试想，如果我们的“%.o”有几百个，那种我们只要用这种很简单的“静态模式规则” 就可以写完一堆规则，实在是太有效率了。“静态模式规则”的用法很灵活，如果用得好， 那会一个很强大的功能。再看一个例子：

```
files = foo.elc bar.o lose.o

$(filter %.o,$(files)): %.o: %.c
$(CC) -c $(CFLAGS) $< -o $@
$(filter %.elc,$(files)): %.elc: %.el
emacs -f batch-byte-compile $< 
```

**$(filter %.o,$(files))表示调用 Makefile 的 filter 函数**，过滤“$filter”集，只要其 中模式为“%.o”的内容。其的它内容，我就不用多说了吧。这个例字展示了 Makefile 中更 大的弹性。



## 自动生成依赖性 

在 Makefile 中，我们的依赖关系可能会需要包含一系列的头文件，比如，如果我们的 main.c 中有一句“#include "defs.h"”，那么我们的依赖关系应该是：

```
main.o : main.c defs.h 
```

但是，如果是一个比较大型的工程，你必需清楚哪些 C 文件包含了哪些头文件，并且， 你在加入或删除头文件时，也需要小心地修改 Makefile，这是一个很没有维护性的工作。 为了避免这种繁重而又容易出错的事情，我们可以使用 C/C++编译的一个功能。大多数的 C/C++编译器都支持一个“-M”的选项，即自动找寻源文件中包含的头文件，并生成一个依 赖关系。例如，如果我们执行下面的命令： 

```
cc -M main.c 
```

其输出是：

```
main.o : main.c defs.h
```

于是由编译器自动生成的依赖关系，这样一来，你就不必再手动书写若干文件的依赖关 系，而由编译器自动生成了。需要提醒一句的是，如果你使用 GNU 的 C/C++编译器，你得用 “-MM”参数，不然，“-M”参数会把一些标准库的头文件也包含进来。

