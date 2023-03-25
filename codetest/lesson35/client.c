//TCP客户端

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){

    //创建套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1){
        perror("socket");
        exit(-1);
    }
    struct sockaddr_in caddr;
    caddr.sin_family=AF_INET;
    caddr.sin_port=htons(9999);
    inet_pton(AF_INET,"127.0.0.1",&caddr.sin_addr.s_addr);

    //connect
    socklen_t len = sizeof(caddr);
    int ret = connect(fd,(struct sockaddr *)&caddr,len);
    if(ret == -1){
        perror("connect");
        exit(-1);
    }

    //通信
    char str[512]={0};
    size_t slen = 0;
    char rdbuf[1024]={0};
    ssize_t rd;
    while((rd=read(STDIN_FILENO,str,sizeof(str)))!=-1){
        write(fd,str,sizeof(str));
        printf("write length:%ld\n",strlen(str));
        read(fd,rdbuf,sizeof(rdbuf));
        printf("%s",rdbuf);
        memset(rdbuf,0,sizeof(rdbuf));
        memset(str,0,sizeof(str));
    }

    close(fd);
    return 0;
}