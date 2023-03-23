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
    inet_pton(AF_INET,"192.168.134.101",&caddr.sin_addr.s_addr);

    //connect
    socklen_t len = sizeof(caddr);
    int ret = connect(fd,(struct sockaddr *)&caddr,len);
    if(ret == -1){
        perror("connect");
        exit(-1);
    }

    //通信
    char rdbuf[1024];
    int i=0;
    while(1){
        sprintf(rdbuf,"data:%d\n",i++);
        write(fd,rdbuf,strlen(rdbuf)+1);
        int lenth = read(fd,rdbuf,sizeof(rdbuf));
        if(lenth == -1){
                    perror("read");
                    exit(-1);
                }else if(len > 0){
                    printf("client recv: %s",rdbuf);
                }else if(len == 0){
                    //printf("connection break\n");
                    break;
                }
        sleep(1);
    }
    close(fd);
    return 0;
}