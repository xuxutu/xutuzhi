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
#include <pthread>

int sockfd;


void sig_handler(int signo)
{
		if(signo == SIGINT)
		{
				printf("server close\n");
				close(sockfd);//关闭socket
				exit(1);
		}

}





void out_fd(int fd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	//从fd获得客户端的相关信息
	if(getpeername(fd, (struct sockaddr*)&addr, &len) < 0)
	{
		perror("getpeername error");
		return;
	}	

	char ip[16];
	memset(ip, 0, sizeof(ip));	
	
	int port = ntohs(addr.sin_port);
	inet_ntop(AF_INET, &addr.sin_addr.s_addr, ip, sizeof(ip));

	printf("%16s(%5d) closed!\n", ip, port);	
}	



void do_service(int fd)
{
	//与客户端进行读写操作		
	char buf[512];
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		puts("start read and write...\n ");
		size_t size;
		if((size = read_msg(fd, buf, sizeof(buf))) < 0)
		{
			perror("protocal error");
			break;
		}
		else if(size ==  0)
			break;
		else
		{
			puts(buf);
			
		
		}

		
	
	}			


	

}

void th_fn(void* arg)
{

	int fd = (int)arg;
	do_service(fd);
	out_fd(fd);	

	close(fd);
}


int main(int argc, char* argv[])
{
		if(argc < 2)
		{
				printf("usage: %s #port\n", argv[0]);
				exit(1);
		}

		if(signal(SIGINT, sig_handler) == SIG_ERR)
		{
				perror("signal sigint error");
				exit(1);
		}


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

		/* 步骤2:调用bind函数将socket和地址(ip、port)进行绑定
		 */
		
		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));	
		//往地址中填入ip、port、internet地址族类型
		serveraddr.sin_family = AF_INET;//IPV4
		serveraddr.sin_port = htons(atoi(argv[1]));
		serveraddr.sin_addr.s_addr = INADDR_ANY;
	
		if(bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
		{
				perror("bind error");
				exit(1);
		}

		/*步骤3: 调用listen函数启动监听(指定port监听)
		 *			 通知系统去接受来自客户端的连接请求
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
		 *			 阻塞直到有客户端连接
		 **/

		struct sockaddr_in clientaddr;
		socklen_t clientaddr_len = sizeof(clientaddr);	

		//设置线程的分离属性	
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);	
		
		while(1)
		{
				int fd = accept(sockfd, NULL, NULL);
				if(fd < 0)
				{
						perror("accept error");
						continue;
				} 
				
				/*步骤5 调用IO函数(read/write)和连接的客户端进行双向的通信	
				 *	
				 **/

				//以分离状态启动子线程
				pthread_t th;	
				int err;
				if((err = pthread_create(&th, &attr, th_fn, (void*)fd)) != 0)
				{
					perror("pthread create errror");
				}

				pthread_attr_destory(&attr);

		}




		return 0;
}
