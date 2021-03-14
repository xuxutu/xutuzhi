#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>

int sockfd;

void sig_handler(int signo)
{
	if(signo == SIGINT)
	{
		printf("server close\n");
		close(sockfd);\
		exit(1);
	}

}


void out_addr(struct sockaddr_in* clientaddr)
{
	char ip[16];
	int port;
	memset(ip, 0, sizeof(ip));
	inet_ntop(AF_INET, &clientaddr->sin_addr.s_addr, ip, sizeof(ip));
	port = ntohs(clientaddr->sin_port);	

	printf("client: %s(%d)\n", ip, port);
}


void do_service(int sockfd)
{
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	//接收数据
	if(recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, &len) < 0)
	{
		perror("recvform error");
		exit(1);
	}
	else
	{
		out_addr(&clientaddr);
		printf("client send into: %s\n", buffer);

		//发送数据
		long int t = time(0);
		char* ptr = ctime(&t);
		size_t size = strlen(ptr) * sizeof(char);
		if(sendto(sockfd, ptr, size, 0, (struct sockaddr*)&clientaddr, len) < 0)
		{
			perror("sendto error");
			exit(1);
		}
	}
}	


int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("usage: %s port\n", argv[0]);
		exit(1);
	}	

	if(signal(SIGINT, sig_handler) == SIG_ERR)
	{
		perror("signal sigint error");
		exit(1);
	}
	
	//first step create socket 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd < 0)
	{
		perror("socket error");
		exit(1);
	}

	int ret;
	int opt = 1;
	
	//设置套接字选项	
	if((ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) < 0)
	{
		perror("setsockopt error");
		exit(1);
	}

	//bind,绑定套接字
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[1]));
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("blind error");
		exit(1);
	}	

	//通讯
	while(1)
	{
		do_service(sockfd);
	}

	return 0;
}
