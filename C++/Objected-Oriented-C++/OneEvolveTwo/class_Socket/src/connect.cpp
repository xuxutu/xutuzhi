#include "client.h"


class MyClient : public Client
{
public:
	MyClient(const char* ip,const char* port) : Client(ip, port)
	{

	}
	
	void handleInfo(int sockfd)
	{
		char buffer[1024];	
		ssize_t size;
		/*
		if((size = read(sockfd, buffer, sizeof(buffer))) < 0)
		{
			perror("read error");
		}		
	
		if(write(STDOUT_FILENO,buffer,size) != size)
		{
			perror("write error");	
		}
		*/
		while(1)
		{
			//发送请求
			if((size = read(STDIN_FILENO, buffer, sizeof(buffer))) < 0)
			{
				perror("read error");
			}
			else
			{
				if((size = write(sockfd, buffer, size)) != size)
				{
					perror("write error");
				}
			}

			//接收信息
			if((size = read(sockfd, buffer, sizeof(buffer))) < 0)	
			{
				perror("read error");
			}
			else
			{
				if((write(STDOUT_FILENO, buffer, size)) != size)
				{
					perror("write error");
				}
			}
		}


		close(sockfd);
	}	


};

int main(int argc, const char* argv[])
{
	if(argc < 3)
	{
		printf("usage: %s ip port\n",argv[0]);
		exit(1);	
	}

	MyClient c1(argv[1],argv[2]);
	c1.startConnect();

}
