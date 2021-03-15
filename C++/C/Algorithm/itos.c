#include<stdio.h>
#include<stdlib.h>

//十进制转为字符串

char buf[10];
int i=0;
void itos(int n)
{
    if(n==0)
    {
        return;
    } 
    
    itos(n/10);
    printf("%d",n%10);//6
    buf[i]=n%10+0x30;
    i++;
}

int main(void)
{


    itos(16666);
    puts("\n");
    puts(buf);





    return 0;     
}
