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


int main(int argc, char** argv)
{
	if(argc < 3)
	{
		printf("usage: %s ip port", argv[0]);
		exit(1);
	}


	//创建socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("socket error");
		exit(1);
	}

	//通信
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &serveraddr.sin_addr.s_addr);

	char buffer[1024] = "hello iotek";

	if(sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("sendto error");
		exit(1);
	}
	else
	{
		memset(buffer, 0, sizeof(buffer));
		size_t size;
		if((size = recv(sockfd, buffer, sizeof(buffer), 0)) < 0)
		{
			perror("recv error");
			exit(1);
		}
		else
		{
			printf("%s\n", buffer);
		}
	}

	close(sockfd);	
	return 0;
}
