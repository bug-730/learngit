/*
    水平触发lt（默认）
    支持阻塞和非阻塞socket，当缓冲区被读了一部分时（未读完），epoll会一直通知。

    边沿触发et（手动设置）
    只支持非阻塞socket，（防止被read或write阻塞住），效率比lt模式高，当缓冲区
    未读完时，epoll只会在第一次通知，之后默认用户已知，后续不再通知，直到缓冲区
    有新的内容写入。
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
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

    //调用epoll_create创建实例
    int epfd = epoll_create(100);

    //将监听的文件描述符加入到epoll实例中
    struct epoll_event epev;
    epev.events=EPOLLIN;
    epev.data.fd=lfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epev);
    
    struct epoll_event epevs[1024];
    while (1)
    {
        int ret = epoll_wait(epfd,epevs,1024,-1);
        if(ret == -1){
            perror("epoll");
            exit(-1);
        }
        for(int i=0;i<ret;i++){
            int curfd=epevs[i].data.fd;
            if(curfd == lfd){
                //说明有新的客户端请求连接
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd,(struct sockaddr *)&cliaddr,&len);
                //设置cfd非阻塞
                int flag = fcntl(cfd,F_GETFL);
                flag|=O_NONBLOCK;
                fcntl(cfd,F_SETFL,flag);
                epev.events=EPOLLIN | EPOLLET;
                epev.data.fd = cfd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);
            }
            else{
                //循环读取所有的数据
                int len = 0;
                char buf[512]={0};
                while((len = read(curfd,buf,sizeof(buf)))>0){
                    printf("read buf :%s\n",buf);
                    write(curfd,buf,len);
                }
                if(len == 0){
                    printf("client closed...\n");
                    epoll_ctl(epfd,EPOLL_CTL_DEL,curfd,NULL);
                    close(curfd);
                }else if(len == -1){
                    if(errno == EAGAIN){
                        printf("data over...\n");
                    }
                    else{
                        perror("read");
                        exit(-1);
                    }
                    
                }
                }
            }
        }
    close(lfd);
    close(epfd);
    return 0;
}