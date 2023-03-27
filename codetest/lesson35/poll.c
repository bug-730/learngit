/*
    struct pollfd {
        int   fd;        //要检测的文件描述符
        short events;     //委托内核检测文件描述符的什么事件
        short revents;    //文件描述符实际发生的事件
    };
    int poll(struct pollfd *fds, nfds_t nfds, int timeout);
        -参数：
            -fds：是一个struct poolfd结构体数组，这是一个需要检测的文件描述符集合
            -nfds：这是第一个参数数组中最后一个有效元素的下标+1
            -timeout：阻塞时长
                0：不阻塞
                -1：阻塞，当检测到需要检测的文件描述符有变化，解除阻塞
                >0:阻塞的时长
        -返回值：
            -1:失败
            >0(n):成功。n表示检测到集合中有n个描述符发生变化
    events:
        POLLIN:读事件
        POLLOUT：写事件


*/

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/poll.h>

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

    //初始化poll数组
    struct pollfd fds[1024];
    for(int i=0;i<1024;i++){
        fds[i].fd=-1;
        fds[i].events=POLLIN;
    }
    fds[0].fd=lfd;
    int nfds = 0;
    while(1){
        //调用poll系统函数，让内核帮忙检测哪些文件描述符有数据
        int ret = poll(fds,nfds + 1,-1);
        if(ret == -1){
            perror("poll");
            exit(-1);
        }else if(ret == 0){
            continue;   //说明没检测到有数据，在永久阻塞的情况下不会发生
        }else if(ret > 0){
            //说明检测到了有文件描述符对应的缓冲区的数据发生改变
            if(fds[0].revents & POLLIN){
                //表示有新的客户端连接
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd,(struct sockaddr *)&cliaddr,&len);
                //将新的文件描述符加入poll数组中
                for(int i=1;i<1024;i++){
                    if(fds[i].fd == -1){
                        fds[i].fd=cfd;
                        fds[i].events=POLLIN;
                        break;
                    }
                }
                nfds = nfds > cfd ? nfds:cfd;  //不严谨，可以直接将nfds设置为固定值
            }

            //遍历后面的文件描述符有无数据变化
            for(int i=1;i<=nfds;i++){
                if(fds[i].revents&POLLIN){
                    //说明这个文件描述符对应的客户端发来了数据
                    char buf[1024]={0};
                    int len = read(fds[i].fd,buf,sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(-1);
                    }else if(len == 0){
                        printf("client close...\n");
                        close(fds[i].fd);
                        fds[i].fd=-1;
                    }else if(len > 0){
                        printf("read buf :%s",buf);
                        write(fds[i].fd,buf,strlen(buf)+1);
                    }
                }
            }
        }
    }
    close(lfd);
    return 0;
}