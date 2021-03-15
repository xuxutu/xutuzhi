#include<stdio.h>

int add(int n)
{
    if(n==1)
    {
        return 1;
    }    

    return n+add(n-1);
}


int main(void)
{

    //递归实现累加
    
    int sum=add(100);
    printf("sum=%d\n",sum);



    return 0;
}


