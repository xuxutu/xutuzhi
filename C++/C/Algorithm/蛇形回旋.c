#include<stdio.h>
#include<stdlib.h>
#define N 9

void snake(int(*a)[N],int start,int len)
{

    if(start>(N*N))
    {
        return;
    }

    //第一步
    int i=N-len;
    int j=N-len;
    int data=start;
    
    while(i!=len)
    {
        a[i][j]=data;
	i++;
	data++;
    }

    //第二步
    i--;
    j++;
    while(j != len)
    {
	    a[i][j]=data;
	    j++;
	    data++;
    }

    //第三步
    j--;
    i--;
    while(i >= (N-len))
    {
        a[i][j]=data;
	i--;
	data++;
    }    

    //第四步
    i++;
    j--;
    while(j>(N-len))
   {
	a[i][j]=data;
	j--;
        data++;
    }

    start=data;
    len--;
    snake(a,start,len);
}

int main(void)
{
    int a[N][N]={0};
    int start=1;
    int len=N;
    snake(a,start,len);

    for(int i=0;i<N;i++)
    {
	    for(int j=0;j<N;j++)
	    {
                printf("%4d",a[i][j]);   
	    }

            printf("\n");  
    }
}

