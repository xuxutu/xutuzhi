#include "server.h"


class MyServer : public Server
{
public:	
	MyServer(const char* port, int ip = INADDR_ANY) : Server(port,ip) 
	{

	}

	void server(int cfd)
	{
		//获得系统时间
		long t =time(0);
		char* s =ctime(&t);
		size_t size = strlen(s) * sizeof(char);
		//将系统时间写回到客户端
		if(write(cfd, s, size) != size)
		{
			perror("write error");
		}	
	
	}



};



int main(int argc, const char* argv[])
{
	if(argc < 2)
	{
		printf("usage: %s #ip(默认情况下系统自动分配) #port\n", argv[0]);
		exit(1);
	}	
	

	MyServer s1(argv[1]);//创建服务
	s1.startServer();//启动服务
	//this指针传入父类函数后发生转型，此时调用虚函数会进行动态绑定，使之调用到子类重写的函数

	return 0;
}
