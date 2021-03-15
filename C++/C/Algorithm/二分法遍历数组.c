#include<stdio.h>

/*二分法输出{1,2,3,4,5,6,7,8,9}
 *
 *
 *
 * */
//递归实现

void _binout(int* a,int L,int mid,int R)
{
    if(L==R)
    {
        printf("%d\n",a[mid]);
    }

    mid=(L+R)/2;
    _binout(a,L,mid);
    _binout(a,mid+1,,R);
}


void  binout(int* a,int n )
{
    int L=0;
    int mid=(L+R)/2;    
    _binout(a,L,mid,R); 
}


int main(void)
{
    int a[10]={1,2,3,4,5,6,7,8,9};

    binout(a,10);

    return 0;
}
