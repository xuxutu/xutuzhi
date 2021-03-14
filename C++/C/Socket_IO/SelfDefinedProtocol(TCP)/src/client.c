#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <signal.h>
#include <time.h>
#include "msg.h"

int main(int argc ,char* argv[])
{
	if(argc < 3)
	{
		printf("usage: %s ip port\n",argv[0]);
		exit(1);	
	}	

	/*步骤1： 创建socket*/
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)		
	{
		perror("socket error");
	}

	//服务端地址
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));	
	//往地址中填入ip、port、internet地址族类型
	serveraddr.sin_family = AF_INET;//IPV4
	serveraddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &serveraddr.sin_addr.s_addr);
	
	//连接到服务器端
	if(connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("connect error");
		exit(1);
	}

	//IO
	char buf[512];
	size_t size;
	char* prompt = ">";
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		write(STDOUT_FILENO, prompt, 1);
		size = read(STDIN_FILENO, buf, sizeof(buf));
		
		if(size < 0)
		{
			continue;
		}

		buf[size-1] = 0;

		if(write_msg(sockfd, buf, sizeof(buf)) < 0)
		{
			perror("write_msg error");
			continue;
		}
		else
		{
			if(read_msg(sockfd, buf, sizeof(buf)) < 0)
			{
				perror("read_msg error");
				continue;
			}
			else
			{
				puts(buf);
			}
		}

	}


	close(sockfd);

	return 0;	
}
