#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

//子进程回收程序
void recycleson(){
    while(1){
        int ret = waitpid(-1,NULL,WNOHANG);
        if(ret == -1||ret == 0){
            break;
        }
        else if(ret > 0){
            printf("回收子进程pid：%d\n",ret);
            break;
        }
    }
}

int main()
{
    //创建信号捕捉
    struct sigaction act;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);
    act.sa_handler=recycleson;
    sigaction(SIGCHLD,&act,NULL);

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
    ret = listen(lfd,128);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }

    //连接
    while(1){
        struct sockaddr_in clientaddr;
        socklen_t slen;
        int fd = accept(lfd,(struct sockaddr *)&clientaddr,&slen);
        if(fd == -1){
            if(errno == EINTR){
                continue;
            }
            perror("accept");
            exit(-1);
        }
        pid_t pid = fork();
        if(pid == 0){
            //子进程
            //输出客户端信息
            char clientIP[16];
            inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,clientIP,sizeof(clientIP));
            unsigned short pt = ntohs(clientaddr.sin_port);
            printf("client IP :%s,port :%d\n",clientIP,pt);
            //消息回射
            char rcvbuf[1024];
            while(1){
                int len = read(fd,rcvbuf,sizeof(rcvbuf));
                if(len == -1){
                    perror("read");
                    exit(-1);
                }else if(len > 0){
                    printf("server recv: %s",rcvbuf);
                    write(fd,rcvbuf,strlen(rcvbuf)+1);
                }else if(len == 0){
                    //printf("connection break\n");
                    break;
                }
            }
            close(fd);
            exit(0);
        }
    }
    return 0;
}

