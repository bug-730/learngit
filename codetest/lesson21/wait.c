/*
#include <sys/types.h>
#include <sys/wait.h>

    pid_t wait(int *status);
    -等待任意一个子进程结束，回收其资源
    参数：进程退出时的状态信息，传入一个int型的地址，传出参数
    返回值：
    成功：返回被回收的子进程id
    失败返回-1（所有子进程都结束，调用函数失败）
    调用wait函数的进程会被挂起（阻塞），直到他的一个子进程退出或者收到一个不能
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
            int ret=wait(NULL);
            if(ret == -1){
                break;
            }
            printf("Child die, pid =%d\n",ret);
            sleep(1);
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