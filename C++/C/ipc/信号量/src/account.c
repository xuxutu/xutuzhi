#include "account.h"
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "pv.h"

//取款
double withdraw(Account* a, double amt)
{
	assert(a != NULL);

	//对信号量集semid的0号信号量做P(1)操作
	P(a->semid, 0, 1); //
	if(amt < 0 || amt > a->balance)
	{
		V(a->semid, 0, 1);
		return 0.0;
	}

	double balance = a->balance;
	balance -= amt;
	a->balance = balance;

	V(a->semid, 0, 1);
	return amt;
}

//存款
double deposit(Account* a, double amt)
{
	assert(a != NULL);

	P(a->semid, 0, 1);
	if(amt < 0)
	{
		V(a->semid, 0, 1);
		return 0.0;
	}

	double balance = a->balance;
	balance += amt;
	a->balance = balance;

	V(a->semid, 0, 1);
	return amt;
}

//查看账户余额
double get_balance(Account* a)
{
	assert(a != NULL);
	P(a->semid, 0, 1);
	double balance = a->balance;
	V(a->semid, 0, 1);
	return balance;
}

		
