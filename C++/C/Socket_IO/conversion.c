#include<stdio.h>
#include<arpa/inet.h>

int main(void)
{
    //128.2.194.242
    //0x8002c2F2
    //F2C20280


    //点分十进制转换为32位小端字节序

    char* address="128.2.194.242";
    int t;
    inet_pton(AF_INET,address,&t);
    printf("%X\n",t);

    int a=0x7f000001;
    char buf[16]={0};
    printf("%s\n",inet_ntop(AF_INET,&a,buf,16));

    //32位小端字节序转换为网络大端字节序
    //printf("%X\n",htonl(t));

    /*十六进制地址           点分十进制地址
     *0x0                    0.0.0.0
     *0xffffffff             255.255.255.255
     *0x7f000001             1.0.0.127

     * */
    return 0;
}
