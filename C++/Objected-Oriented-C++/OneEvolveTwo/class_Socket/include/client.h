#ifndef __CLIENT__
#define __CLIENT__
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
#include <iostream>
#include <fcntl.h>
using namespace std;

class Client
{
public:
	Client(const char* ip,const char* port)	: m_ip(ip), m_port(port)
	{
		/*步骤1： 创建socket*/
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0)		
		{
			perror("socket error");
		}
	}

	void startConnect();

	virtual void handleInfo(int sockfd){};

private:
	int sockfd;
	const char* m_ip;
	const char* m_port;
};




#endif
