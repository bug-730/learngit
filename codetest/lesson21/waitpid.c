/*
#include <sys/types.h>
#include <sys/wait.h>


       pid_t waitpid(pid_t pid, int *status, int options);
            功能：回收指定进程好的子进程，可设置是否阻塞
            参数：
                -pid:
                    >0：某个子进程的pid
                    =0：回收当前进程组的所有子进程
                    =-1：回收所有子进程，相当于wait() （最常用）
                    <-1：回收某个进程组的组id的绝对值，回收指定进程组中的组id
                -options：
                    0：阻塞
                    WNOHANG：非阻塞
            返回值：
                >0:返回子进程id
                =0：options=WNOHANG，表示还有子进程或者
                -1；错误，表示没有子进程活着

*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
int main(){

    pid_t pid;

    for(int i=0;i<5;i++){
        pid=fork();
        if(pid == 0){
            break;
        }                       //如果是子进程则不fork
    }

    if(pid>0){
        while(1){
            printf("parent ,pid =%d\n",getpid());
            sleep(15);
            int st;
            int ret=waitpid(-1,&st,WNOHANG);
            if(ret == -1){
                break;
            }
            if(ret==0){
                continue;
            }
            else if(ret>0){
                if(WIFEXITED(st)){
                printf("进程正常退出，退出状态码为：%d\n",WEXITSTATUS(st));
            }
                if(WIFSIGNALED(st)){
                printf("被那个信号干掉了：%d\n",WTERMSIG(st));
            }
                printf("Child die, pid =%d\n",ret);
            }
        }
        
    }
    else if(pid == 0){
        while(1){
            printf("child , pid =%d\n",getpid());
            sleep(3);
        }
    }
    return 0;
}