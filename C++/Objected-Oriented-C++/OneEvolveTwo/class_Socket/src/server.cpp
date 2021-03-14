#include "server.h"

Server::Server(const char* port, int ip) : m_port(port), m_ip(ip)
{
	/* 步骤1: 创建socket
	 * 注: socket创建在内核中，是一个结构体
     * AF_INET: IPV4
	 * SOCK_STREAM: tcp协议
	 **/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{   
        perror("socket error");
        exit(1);
	}

	if(signal(SIGINT, sig_handler) == SIG_ERR)
    {   
        perror("signal sigint error");
        exit(1);
    }


}

void Server::startServer()
{
	/* 步骤2:调用bind函数将socket和地址(ip、port)进行绑定
	 */

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));	
	//往地址中填入ip、port、internet地址族类型
	serveraddr.sin_family = AF_INET;//IPV4
	serveraddr.sin_port = htons(atoi(m_port));
	serveraddr.sin_addr.s_addr = m_ip;
	
	if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("bind error");
		exit(1);
	}

	/*步骤3: 调用listen函数启动监听(指定port监听)
	 *		 通知系统去接受来自客户端的连接请求
	 *       (将接受到的客户端连接请求放置到对应的队列当中)
	 *	     指定队列的长度
	 **/ 	

	if(listen(sockfd, 2048) < 0)
	{
		perror("listen error");
		exit(1);
	}
	
	/*步骤4: 调用accept函数从队列中获得一个客户端的请求连接
	 *       并返回新的描述符(用于通讯)
	 *		 阻塞直到有客户端连接
	 **/

	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len = sizeof(clientaddr);	
	while(1)
	{
		int fd = accept(sockfd, (struct sockaddr*)&clientaddr, &clientaddr_len);
		if(fd < 0)
		{
			perror("accept error");
			continue;
		} 

		/*步骤5 调用IO函数(read/write)和连接的客户端进行双向的通信	
		 *	
		 **/

		showClientAddr(&clientaddr);
		server(fd);

		//步骤6: 关闭socket
		close(fd);				
	}
	


}

void Server::showClientAddr(struct sockaddr_in* clientaddr)
{
	//将端口从网络字节转为主机字节序
	int port = ntohs(clientaddr->sin_port);
	char ip[16];
	memset(ip, 0, sizeof(ip));
	//将ip从网络字节序转换为点分十进制
	inet_ntop(AF_INET, &clientaddr->sin_addr.s_addr, ip, sizeof(ip));
	printf("client: %s:%d connected\n", ip, port);

}


void Server::sig_handler(int signo)
{
	if(signo == SIGINT)
	{
		printf("\nserver close\n");
		//close((st->ths)->sockfd);//关闭socket
		exit(1);
	}

}
