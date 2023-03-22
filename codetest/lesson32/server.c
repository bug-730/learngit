//TCP通信的服务器端
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){

    //1.创建监听套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd == -1){
        perror("socket");
        exit(-1);
    }
    
    //2.绑定
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    //inet_pton(AF_INET,"192.168.1.101",saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr=INADDR_ANY;
    saddr.sin_port=htons(9999);
    int ret = bind(lfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    //3.监听
    ret = listen(lfd,8);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }

    //4.接收客户端连接
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int cfd = accept(lfd,(struct sockaddr *)&clientaddr,&len);
    if(cfd == -1){
        perror("accept");
        exit(-1);
    }

    //输出客户端信息
    char clientIP[16];
    inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,clientIP,sizeof(clientIP));
    unsigned short pt = ntohs(clientaddr.sin_port);
    printf("client IP :%s,port :%d\n",clientIP,pt);

    //5.通信
    //获取消息
    char rdbuf[1024]={0};
    while((ret=read(cfd,rdbuf,sizeof(rdbuf)))!=0){
        printf("read length:%ld\n",strlen(rdbuf));
        write(cfd,rdbuf,strlen(rdbuf));
        memset(rdbuf,0,sizeof(rdbuf));
    }
    
    //关闭文件描述符
    close(lfd);
    close(cfd);
    return 0;
}