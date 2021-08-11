# 高级IO



## 1.非阻塞IO



### 1.1 有限状态机

解决复杂流程

![image-20210724142003511](senior_io.assets/image-20210724142003511.png)	

两个设备的读写描述：

最开始为读态，读出错分两种情况：eagin,继续读，ERR，到异常态。一次读完接着写，读取完毕直接结束态。

写出错到异常态，写eagin继续写，写不够，继续写。写完继续读。

## 

## 2.IO多路转接

监视多个文件描述符的行为

### 2.1 select()

//移植性好

sleep不太靠谱，有些平台用alarm，程序当中不能出现两个时钟。而select的副作用可以设置一个安全的休眠

select(-1, 0, 0, 0, time) = sleep 安全休眠

       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
                  
        nfds，所监视的文件描述符中最大的那个+1
        readfds，你所关心的可读的描述符
        writefds，你所关心的可写的描述符
        exceptfds 你所关心的异常的描述符
        timeout  超时设置 不设置，则死等 
             
        返回值 你所关心的三类描述符的个数     
                  
       void FD_CLR(int fd, fd_set *set);//删除
       int  FD_ISSET(int fd, fd_set *set);//判断
       void FD_SET(int fd, fd_set *set);//添加
       void FD_ZERO(fd_set *set);//清空


### 2.2 poll();

//可移植

### 2.3 epoll()//linux 

方言基于poll的实现;