/*
#include <unistd.h>

    int pipe(int pipefd[2]);
        功能：创建一个匿名管道，用来进程间通信
        参数：int pipefd[2]是一个传出参数
            pipefd[0] 对应的管道读端
            pipefd[1] 对应的是管道的写端
        返回值：
            成功：0
            失败：-1
        管道默认是阻塞的，管道无数据则read阻塞，满了则write阻塞
    注意：匿名管道只能用于具有关系的进程之间的通信（父子进程，兄弟进程）
*/
//子进程向父进程发送数据，父进程收到数据输出
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(){
    //创建一个管道，在fork之前
    int pipefd[2];
    int ret=pipe(pipefd);
    if(ret==-1){
        perror("pipe");
        exit(0);
    }
    pid_t pid=fork();

    if(pid > 0){
        //父进程
        printf("i am parent process, pid :%d\n",getpid());
        char buf[1024]={0};
        char *str="hello i am parent!";
        //父进程关闭写端
        close(pipefd[1]);
        while(1){
            //写
            // write(pipefd[1],str,strlen(str));
            // sleep(1);
            //读
            int len = read(pipefd[0],buf,sizeof(buf));
            printf("parent recv buf:%s\n",buf);
            bzero(buf,1024);
        }
    }

    else if(pid == 0){
        //子进程
        printf("i am child process, pid :%d\n",getpid());
        char buf[1024]={0};
        char *str="hello i am child!\n";
        //子进程关闭读端
        close(pipefd[0]);
        while(1){
            //读
            // int len = read(pipefd[0],buf,sizeof(buf));
            // printf("child recv buf:%s\n",buf);
            // bzero(buf,1024);
            //写
            write(pipefd[1],str,strlen(str));
            //sleep(1);
        }
       
    }
    return 0;
}