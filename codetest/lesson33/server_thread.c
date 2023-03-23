#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define N 128 //最大连接数



//封装传参结构体
struct sockInfo{
    int fd;//通信的文件描述符
    struct sockaddr_in addr;//通信的socket地址
    pthread_t tid; //线程号    
};

//创建一个sockInfo数组
struct sockInfo sockInfos[N];

void *working(void *arg){
    //子线程和客户端通信
    struct sockInfo *pInfo = (struct sockInfo *)arg;
    char clientIP[16];
    inet_ntop(AF_INET,&pInfo->addr.sin_addr.s_addr,clientIP,sizeof(clientIP));
    unsigned short pt = ntohs(pInfo->addr.sin_port);
    printf("client IP :%s,port :%d\n",clientIP,pt);
    //消息回射
    char rcvbuf[1024];
    while(1){
        int len = read(pInfo->fd,rcvbuf,sizeof(rcvbuf));
        if(len == -1){
            perror("read");
            exit(-1);
        }else if(len > 0){
            printf("server recv: %s",rcvbuf);
            write(pInfo->fd,rcvbuf,strlen(rcvbuf)+1);
        }else if(len == 0){
            printf("connection break,tid:%ld\n",pInfo->tid);
            break;
        }
    }
    close(pInfo->fd);
    pthread_exit(NULL);
}


int main(){
    //创建套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(9999);
    saddr.sin_addr.s_addr=INADDR_ANY;
    
    //绑定
    int ret = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    //监听
    ret = listen(lfd,N);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }

    //初始化连接数组
    for(int i=0;i<N;i++){
        bzero(&sockInfos[i],sizeof(sockInfos[i]));
        sockInfos[i].fd = -1;  //代表该sockInfo可用
        sockInfos[i].tid = -1;
    }

    //循环等待客户端连接，一旦一个子线程进来，就创建一个子线程通信
    while(1){
        struct sockaddr_in clientaddr;
        socklen_t slen;
        int fd = accept(lfd,(struct sockaddr *)&clientaddr,&slen);
        
        //需要向working中传参，将需要的信息封装一个struct传入
        struct sockInfo *pInfo;
        //遍历sockInfo数组，找到可用的sockInfo
        for(int i=0;i<N;i++){
            if(sockInfos[i].fd == -1){
                pInfo=&sockInfos[i];
                break;
            }
            if(i == N-1){
                sleep(1);
                i--;
            }
        }
        pInfo->fd=fd;
        memcpy(&pInfo->addr,&clientaddr,sizeof(clientaddr));

        pthread_create(&pInfo->tid,NULL,working,pInfo);
        pthread_detach(pInfo->tid);
    }
    close(lfd);
    return 0;
}