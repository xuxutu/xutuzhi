#include<stdio.h>
#define N 3

/*
 1 2 3
 8 9 4
 7 6 5
*/

void snake(int (*a)[N],int start,int n)
{
    if(n==0)
    {
    
        return ;    
    }
    //First
    
    int i=0;
    int j=0;   

    while(j<n)
    {
	a[i][j]=start;
	start++;
	j++;
    }

    start--;
    j--;

    //Second
    while(i<n)
    {
	a[i][j]=start; 
        start++;
        i++;	
    } 
    
    start--;
    i--;

    //third
    while(j>=0)
    {
        a[i][j]=start;
        start++;
	j--;
    }
   
    j++; 
    i--;
    //Fourth
   
    while(i>0)
    {
        a[i][j]=start;
        i--;
    }

    snake(a,start,n-1);

    return;

}

void print(int (*a)[N],int n)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
	{
	    printf("%5d",a[i][j]);
	}
	printf("\n");  
    }

    return;
}

int main(void)
{
    int a[N][N]={0};
    int start=1;

    snake(a,start,N);
    print(a,N); 
    


    return 0;
}
