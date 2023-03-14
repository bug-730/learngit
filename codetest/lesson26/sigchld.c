/*
1.子进程结束
2.子进程被暂停
3.子进程从暂停态恢复
都会给父进程发送SIGCHLD信号


    --使用SIGCHLD信号可以解决僵尸进程问题

    
*/
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/wait.h>
void Myfunc(int num){
    printf("捕捉到信号 :%d\n",num);
    while(1){
        int ret =waitpid(-1,NULL,WNOHANG);
        if(ret > 0){
            printf("child die :%d\n",ret);
        }else if(ret == 0||ret == -1){
            break;
        }
    }
}

/*
struct sigaction {
    void     (*sa_handler)(int);        //函数指针，指向处理函数
    void     (*sa_sigaction)(int, siginfo_t *, void *); //不常用
    sigset_t   sa_mask; //临时阻塞信号集，在信号捕捉函数执行过程中，临时阻塞某些信号
    int        sa_flags;    //使用哪一个信号处理函数对信号处理
    若为0，使用sa_handler,若为SA_SIGINFO,表示使用sa_sigaction
    void     (*sa_restorer)(void); //已废弃，传NULL即可
    };
     int sigaction(int signum, const struct sigaction *act,
                struct sigaction *oldact);
*/
int main(){
    //注册信号捕捉
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    act.sa_handler=Myfunc;
    sigaction(SIGCHLD,&act,NULL);
    //创建子进程
    pid_t pid;
    for(int i=0;i<20;i++){
        pid=fork();
        if(pid == 0){
            break;
        }
    }

    if(pid > 0){
        //父进程
            printf("parent process:%d\n",getpid());
        while(1){
            sleep(1);
        }
    }

    if(pid == 0){
        printf("child procecss :%d\n",getpid());
    }
    return 0;
}