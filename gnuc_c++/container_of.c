#include <stdio.h>
#include <stdlib.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({ \
	const typeof(((type *)0)->member) * __mptr = (ptr); \
	(type *)((char *)__mptr - offsetof(type, member)); })



#define to_urb(d) container_of(d, struct urb, kref) 


//container_of(dev, struct drv2625_data, dev);

#pragma pack (4)

struct kref
{
	double son;
};

struct urb
{
	int a;
	struct kref kref;
	int c;
	int d;
};


/*

	const struct kref* __mptr = kref; 
	(char*)__mptr - offsetof(type, member) = 
	offsetof(TYPE, MEMBER)
	&(struct urb *)0.kref 

	
	container_of(ptr, type, member)
	container_of(d, struct urb, kref)

*/


int main()
{

	struct kref* kref = malloc(sizeof(struct kref));

	struct urb* urb = to_urb(kref);

	printf("kref = %ld\n", (long)kref);
	printf("urb =  %ld\n", (long)urb);
	printf("kref - urb = %ld\n", offsetof(struct urb, kref));
	printf("sizeof(urb) = %ld\n", sizeof(struct urb));

	printf("%ld\n", (char*)kref - (char*)&(((struct urb*)0)->kref));

	

//	printf("%ld\n", (size_t)((char* )kref - (char*)(&(struct urb*)0)->kref));

	return 0;
}
