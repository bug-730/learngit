/*
    主旨思想：
    1.首先构造一个关于文件描述符的列表，将要监听的文件描述符添加到该列表中
    2.调用一个系统函数，监听该列表中的文件描述符，直到这些描述符中的一个或者
    多个进行I/O操作时，该函数才返回
        a.这个函数是阻塞的
        b.函数对文件描述符的检测的操作是由内核完成的
    3.在返回时，它会告诉进程有多少描述符要进行I/O操作
        #include <sys/select.h>

       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
            参数：
                -nfds：委托内核检测的最大文件描述符的值+1
                -readfds：要检测的文件描述符的读的集合，委托内核检测哪些文件描述符的读的属性
                        - 一般检测读操作
                        - 对应的是对方发送过来的数据，因为读是被动的接收数据，检测的就是读缓冲区的
                        -是一个传入传出参数
                -writefds：要检测的文件描述符的写的集合，委托内核检测哪些文件描述符的写的属性
                        - 委托内核检测写缓冲区是不是还可以写数据（不满的就可以写）
                -exceptfds：检测发生异常的文件描述符的集合
                -timeout：设置的超时时间
                struct timeval {
                  time_t      tv_sec;  
                  suseconds_t tv_usec; 
                };
                -NULL：永久阻塞，直到检测到了文件描述符有变化
                =0：不阻塞

            //sizeof(fd_set) = 128 1024位
            返回值：
                - -1：失败
                - >0(n)：检测的集合中有n个文件描述符发生了变化

       void FD_CLR(int fd, fd_set *set);
            将参数文件描述符fd对应的标志位设置为0
       int  FD_ISSET(int fd, fd_set *set);
            判断fd对应的标志位是0还是1，返回值：fd对应的标志位的值
       void FD_SET(int fd, fd_set *set);
            将参数文件描述符fd，对应的标志位设置为1
       void FD_ZERO(fd_set *set);
            将1024位都设置为0

*/

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>

int main(){

    //创建socket
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=INADDR_ANY;
    saddr.sin_port=htons(9999);

    //绑定
    int bindret = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(bindret == -1){
        perror("bind");
        exit(-1);
    }
    //监听
    listen(lfd,8);

    //创建一个fd_set的集合，存放的是需要检测的文件描述符
    fd_set rdset,tmp;
    FD_ZERO(&rdset);
    FD_SET(lfd,&rdset);
    int maxfd = lfd;
    while(1){
        tmp=rdset;
        //调用select系统函数，让内核帮忙检测哪些文件描述符有数据
        int ret = select(maxfd + 1,&tmp,NULL,NULL,NULL);
        if(ret == -1){
            perror("select");
            exit(-1);
        }else if(ret == 0){
            continue;   //说明没检测到有数据，在永久阻塞的情况下不会发生
        }else if(ret > 0){
            //说明检测到了有文件描述符对应的缓冲区的数据发生改变
            if(FD_ISSET(lfd,&tmp)){
                //表示有新的客户端连接
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd,(struct sockaddr *)&cliaddr,&len);
                //将新的文件描述符加入set中
                FD_SET(cfd,&rdset);
                maxfd = maxfd > cfd ? maxfd:cfd;
            }

            //遍历后面的文件描述符有无数据变化
            for(int i=lfd+1;i<=maxfd;i++){
                if(FD_ISSET(i,&tmp)){
                    //说明这个文件描述符对应的客户端发来了数据
                    char buf[1024]={0};
                    int len = read(i,buf,sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(-1);
                    }else if(len == 0){
                        printf("client close...\n");
                        close(i);
                        FD_CLR(i,&rdset);
                    }else if(len > 0){
                        printf("read buf :%s",buf);
                        write(i,buf,strlen(buf)+1);
                    }
                }
            }
        }
    }
    close(lfd);
    return 0;
}