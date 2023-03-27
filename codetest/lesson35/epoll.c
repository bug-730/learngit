/*
    #include <sys/epoll.h>
    //创建一个新的epoll实例。在内核中创建了一个数据，这个数据中有两个比较重要的数据，
    一个是需要检测的文件描述符的信息（红黑树），还有一个是就绪列表，存放检测到数据发送
    改变的文件描述符信息（双向链表）。
    int epoll_create(int size);
        参数：
            size：目前已被忽略，随便一个数即可，但必须大于0
        返回值：
            -1：失败
            >0:文件描述符，操作epoll实例的
    //对epoll实例进行管理：添加文件描述符信息，删除信息，修改信息
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
        参数：
            epfd：epoll实例对应的文件描述符
            op：要进行什么操作
                EPOLL_CTL_ADD：添加
                EPOLL_CTL_MOD：修改
                EPOLL_CTL_DEL：删除
            fd：要检测的文件描述符的值
            event：检测文件描述符什么事情
    struct epoll_event {
    uint32_t     events;      
    epoll_data_t data;       
};
    常见的Epoll检测事件：
        EPOLLIN
        EPOLLOUT
        EPOLLERR
    typedef union epoll_data {
    void        *ptr;
    int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;
    int epoll_wait(int epfd, struct epoll_event *events,
                int maxevents, int timeout);
        参数：
            epfd：epoll实例对应的文件描述符
            events：传出参数，保存了发生了变化的文件描述符的信息
            maxevents:第二个参数结构体数组的大小
            timeout：阻塞时间
                0：不阻塞
                -1：阻塞，直到检测到fd数据发生变化
                >0: 阻塞时间
        返回值：
            成功：返回发生变化的文件描述符的个数>0
            失败：-1

*/

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>

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

                epev.events=EPOLLIN;
                epev.data.fd = cfd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);
            }
            else{
                //有数据到达
                char buf[1024]={0};
                    int len = read(curfd,buf,sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(-1);
                    }else if(len == 0){
                        printf("client close...\n");
                        epoll_ctl(epfd,EPOLL_CTL_DEL,curfd,NULL);
                        close(curfd);
                    }else if(len > 0){
                        printf("read buf :%s",buf);
                        write(curfd,buf,strlen(buf)+1);
                    }
                }
            }
        }
    close(lfd);
    close(epfd);
    return 0;
}