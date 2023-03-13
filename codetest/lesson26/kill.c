/*
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>




int kill(pid_t pid, int sig);
    功能：给任何的进程或者进程组发送信号
    参数：
        pid：
            >0:将信号发送给指定的进程
            =0:将信号发送给当前的进程组
            =-1:将信号发送给每一个有权限接受这个信号的进程
            <-1:这个pid取反的进程组
        sig：要发送的信号或宏值，若为0则不发送信号
int raise(int sig);
    功能：给当前进程发送信号
    参数：sig：要发送的信号
    返回值：成功0，失败非0

void abort(void);
    功能：发送SIGABRT信号给当前进程，杀死当前进程

*/
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int main(){
    pid_t pid = fork();

    if(pid > 0){
        //父进程
        printf("parent process\n");
        sleep(2);
        printf("kill the child process now\n");
        kill(pid,SIGINT);
    }
    else if(pid == 0){
        //子进程
        for(int i=0;i<5;i++){
            printf("child process\n");
            sleep(1);
        }
    }
    
    
    return 0;

}