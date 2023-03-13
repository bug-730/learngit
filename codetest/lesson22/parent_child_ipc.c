/*
实现 ps aux | grep xxx 父子进程间通信
子进程ps aux，子进程结束后，将数据发送给父进程
父进程收到数据并过滤

pipe();
execlp();
子进程将标准输出重定向到 stduot_fileno 重定向到管道的写端  dup2
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
    //创建管道
    int pipefd[2];
    int ret=pipe(pipefd);
    if(ret==-1){
        perror("pipe");
        return -1;
    }

    //创建子进程
    pid_t pid=fork();
    if(pid > 0){
        //父进程
        //关闭写端
        close(pipefd[1]);
        //从管道中直接读取
        char buf[1024]={0};
        int len = -1;
        while((len=read(pipefd[0],buf,sizeof(buf)-1))>0){
            printf("%s",buf);
        }
        wait(NULL);
        //过滤数据
    }

    else if(pid ==0){
        //子进程
        //关闭读端
        close(pipefd[0]);
        //文件描述符的重定向
        dup2(pipefd[1],STDOUT_FILENO);
        //执行ps aux
        execlp("ps","ps","aux",NULL);
        perror("execlp");
        exit(0);
    }
    else{
        perror("fork");
        exit(0);
    }

    return 0;
}