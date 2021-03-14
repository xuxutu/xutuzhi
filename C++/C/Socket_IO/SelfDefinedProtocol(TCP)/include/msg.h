#ifndef __MSG_H__
#define __MSG_H__
#include <sys/types.h>


typedef struct
{
	//procotol header
	char head[10];
	char checknum; //校验吗
	//procotol body
	char buf[512]; //数据

}Msg;


extern int write_msg(int sockfd, char* buf, size_t len);


extern int read_msg(int sockfd, char* buf, size_t len);



#endif
