#include <stdio.h>

#define debug(format, arg...) fprintf (stderr, format, ##arg) 

int main()
{


	debug("%s","a message!\n", "22222", "2222");




	return 0;
}
