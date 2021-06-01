#include "server.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
class MyServer : public Server
{
public:	
	MyServer(const char* port, int ip = INADDR_ANY) : Server(port,ip) 
	{

	}
	
	static void* read_date(void* arg)
	{
		int len;
		char buf[1024];
		int cfd = *(int*)arg;

		while((len = read(cfd, buf, 1024)) > 0)
		{
			if(write(1, buf, len) != len)
			{
				perror("write error");
			}
		}

		pthread_exit(0);
	}

	void server(int cfd)
	{
		//获得系统时间
/*
		long t =time(0);
		char* s =ctime(&t);
		size_t size = strlen(s) * sizeof(char);
		//将系统时间写回到客户端
		if(write(cfd, s, size) != size)
		{
			perror("write error");
		}
*/

		pthread_t pid;
		int err;
		err = pthread_create(&pid, NULL, read_date, &cfd);
		if(err)
		{
			cout<<"err = "<<err<<endl; 	
			fprintf(stderr, "pthread_create error: %s\n", strerror(err));
			exit(1);
		}


		int len;
		char buf[1024];
		int fd = open("test.html", O_RDONLY);
		if(fd < 0)
		{
			perror("open error");
			exit(1);
		}
			
		write(cfd, "HTTP/1.1 200 Ok\r\n", strlen("HTTP/1.1\r\n"));
		write(cfd, "Content-Type: text/plain\r\n", strlen("Content-Type: text/plain\r\n"));
		write(cfd, "Content-Length:-1\r\n", strlen("Content-Length:-1\r\n"));
		write(cfd, "\r\n", 2);	
		while((len = read(fd, buf, 1024)) > 0)
		{
			if(write(cfd, buf, len) != len)
			{
				perror("write error");
			}	
		}

		pause();
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
