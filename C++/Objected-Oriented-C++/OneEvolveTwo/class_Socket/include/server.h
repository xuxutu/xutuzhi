#ifndef __SERVER__
#define __SERVER__
#include <iostream>
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
#include <string>
using namespace std;


class Server
{
public:
	Server(const char* port, int ip = INADDR_ANY);

	virtual void server(int cfd){}//子类应写出自己对应的服务	

	void showClientAddr(struct sockaddr_in* clientaddr);

	void startServer();

	//信号处理函数
	static void sig_handler(int signo);

private:
	int sockfd;
	const char* m_port;
	int m_ip;
};


#endif
