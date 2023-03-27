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
    while(1){
        char wtbuf[1024]={0};
        fgets(wtbuf,sizeof(wtbuf),stdin);
        write(fd,wtbuf,strlen(wtbuf)+1);

        //接收
        int rlen = read(fd,wtbuf,sizeof(wtbuf));
        if(rlen == -1){
            perror("read");
            exit(-1);
        }else if(rlen > 0){
            printf("read buf :%s\n",wtbuf);
        }else{
            printf("server closed...\n");
            break;
        }
    }

    close(fd);
    return 0;
}