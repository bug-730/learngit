/*
#include <unistd.h>

pid_t fork(void);
    返回值：返回两次
    父进程中返回子进程ID
    子进程中返回0
    通过fork返回值区分父进程子进程
    子进程创建失败在父进程中返回-1，设置errno
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
int main(){
    pid_t pid=fork();

    //判断是父进程还是子进程
    if(pid>0){
        printf("pid : %d\n",pid);
        //大于0是父进程
        printf("i am parent process,pid : %d, ppid : %d\n",getpid(),getppid());
        for(int i=0;i<10;i++){
        printf("i : %d\n",i);
        sleep(1);
        }
    }
    else if(pid==0){
        //等于0是子进程
        printf("i am son process,pid : %d, ppid : %d\n",getpid(),getppid());
        for(int j=0;j<10;j++){
        printf("j : %d\n",j);
        sleep(1);
        }
    }
    return 0;
}