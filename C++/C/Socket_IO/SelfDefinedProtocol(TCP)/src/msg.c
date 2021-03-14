#include "msg.h"
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <sys/types.h>


//calculate checknum;
static unsigned char msg_check(Msg* message)
{
	unsigned char s = 0;
	int i;
	for(i = 0; i < sizeof(message->head); i++)
	{
		s += message->head[i];
	}

	for(i = 0; i < sizeof(message->buf); i++)
	{
		s += message->buf[i];
	}

} 





int write_msg(int sockfd, char* buf, size_t len)
{
	Msg message;
	
	memset(&message, 0, sizeof(message));
	strcpy(message.head, "iotek2012");
	memcpy(message.buf, buf, len);
	message.checknum = msg_check(&message);	
	
	if(write(sockfd, &message, sizeof(message)) != sizeof(message))
	{
		return -1;
	}

}

int read_msg(int sockfd, char* buf, size_t len)
{
	Msg message;
	memset(&message, 0, sizeof(message));
	size_t size;
	if((read(sockfd, &message, sizeof(message))) < 0)
	{
		return -1;
	}
	else if(size == 0)
	{
		return 0;
	}
	
	//Judge whether the received data is complete
	unsigned char s = msg_check(&message);
	if((s == (unsigned char)message.checknum) && (!strcmp("iotek2012", message.head)))
	{
		memcpy(buf, message.buf, len);
		return sizeof(message);
	}


	return -1;
}


