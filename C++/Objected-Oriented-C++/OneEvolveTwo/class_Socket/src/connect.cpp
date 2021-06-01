#include "client.h"


class MyClient : public Client
{
public:
	MyClient(const char* ip,const char* port) : Client(ip, port)
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

		pthread_t pid;
		int err;
		err = pthread_create(&pid, NULL, read_date, &sockfd);
		if(err)
		{
			cout<<"err = "<<err<<endl; 	
			fprintf(stderr, "pthread_create error: %s\n", strerror(err));
			exit(1);
		}

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

	/*		//接收信息
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
	*/	
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
