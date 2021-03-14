#include "client.h"


void Client::startConnect()
{
	//服务端地址
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));	
	//往地址中填入ip、port、internet地址族类型
	serveraddr.sin_family = AF_INET;//IPV4
	serveraddr.sin_port = htons(atoi(m_port));
	inet_pton(AF_INET, m_ip, &serveraddr.sin_addr.s_addr);
	
	//连接到服务器端
	if(connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("connect error");
		exit(1);
	}

	handleInfo(sockfd);

}





