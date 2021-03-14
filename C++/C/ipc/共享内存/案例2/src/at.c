#include "account.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	char name[20];
	Account* account;
	double amt;
}OperArg;


void* withdraw_fn(void *arg)
{
	OperArg* oa = (OperArg*)arg;
	
	double amt = withdraw(oa->account, oa->amt);

	printf("%8s(0x%lx) withdraw %f from account %d\n", oa->name, pthread_self(), amt, oa->account->code);

	return (void*)0;
}


void* deposit_fn(void* arg)
{
	
	OperArg* oa = (OperArg*)arg;
	
	double amt = deposit(oa->account, oa->amt);

	printf("%8s(0x%lx) deposit %f from account %d\n", oa->name, pthread_self(), amt, oa->account->code);

	return (void*)0;

}

int main()
{
	int err;
	pthread_t boy,girl;	
	
	Account* a = create_account(100001, 10000);
	OperArg o1, o2;
	strcpy(o1.name, "boy");
	o1.account = a;	
	o1.amt = 10000;

	strcpy(o2.name, "girl");
	o2.account = a;
	o2.amt = 10000;

	//启动两个线程
	if((err = pthread_create(&boy, NULL, withdraw_fn, (void*)&o1)) != 0)
	{
		perror("pthread_create");
	}

	
	//启动两个线程
	if((err = pthread_create(&girl, NULL, withdraw_fn, (void*)&o1)) != 0)
	{
		perror("pthread_create");
	}

	pthread_join(boy,NULL);
	pthread_join(girl,NULL);

	printf("account balance: %f\n", get_balance(a));
	destroy_account(a);

	return 0;
}
