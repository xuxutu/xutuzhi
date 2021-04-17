#include "setenv.h"

extern char** environ;
static char **lastenv;

static char* __findenv(const char* name, int len, int* offset)
{
	int i;
	const char* np;
	char** p, **cp;

		puts("找到了");
	if(name == NULL || environ == NULL)
	{
		return NULL;
	}

	for (p = environ + *offset; (cp = *p) != NULL; ++p)
	{
		puts("找到了");
		for (np = name, i = len; i && *cp; i--)
		{
			//字符串比较函数
			if (*cp++ != *np++)
				break;
		}

		puts("找到了");

		//如果找到
		if(i == 0 && *cp++ == '=')	
		{
			*offset = p - environ;//回填所找环境变量在表中的索引
			return cp;//cp = value
		}
	}

	return NULL;
}

int setenv(const char *name, const char *value, int rewrite)
{
	puts("my setenv start---");

	char*C, **P;	
	const char* np;
	int l_value, offset = 0;
	
	//name为空 或者 name所指的字符串为空，退出
	if (!name || !*name)
	{
		return -1;
	}

	puts("定位");
	//寻找名字里的 =
	for (np = name; *np && *np != '='; ++np);

	if (*np)
	{
	puts("定位");
		/* has `=' in name */
		return -1;
	}	


	l_value = strlen(value); //获取value的长度

	if ((C = __findenv(name, (int)(np - name), &offset)) != NULL)
	{
		//如果找到
		int tmpoff = offset + 1;
		if (!rewrite)
		{
			//如果rewrite为假
			return 0;
		}

		//* could be set multiple times */
		while (__findenv(name, (int)(np - name), &tmpoff))
		{	
			for (P = &environ[tmpoff];; ++P)
			{
				//每找到一次就开始向前覆盖
				if (!(*P = *(P + 1)))
					break;
			}
		}
		
	}
	else
	{
		puts("not find!");	
		//如果没找到，创建新的环境变量
		size_t cnt;
		//找到环境表底部
		for (P = environ; *P != NULL; P++);
		//获取索引
		cnt = P - environ;
		//堆上重新连续内存，此处lastenv=0,等同于malloc
		P = realloc(lastenv, (cnt + 2) * sizeof(char*));
		if(!P)
			return -1;
			
		if (lastenv != environ)
		{
			//搬运
			memcpy(P, environ, cnt * sizeof(char *));
		}

		lastenv = environ = P;

		offset = cnt;

		environ[cnt + 1] = NULL;
	}

	//给key = value分配内村	
	if (!(environ[offset] = malloc((size_t)((int)(np - name) + l_value + 2))))
		return (-1);

	//复制key
	for (C = environ[offset]; (*C = *name++) && *C != '='; ++C);
	
	//复制value
	for (*C++ = '='; (*C++ = *value++); );

	
	puts("my setenv end---");
	return NULL;
}
