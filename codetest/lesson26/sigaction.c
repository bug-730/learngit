/*
    #include <signal.h>

    int sigaction(int signum, const struct sigaction *act,
                struct sigaction *oldact);
        功能：捕捉信号
        参数：
            signum：要捕捉的信号，或宏值
            act：捕捉到信号之后的处理
            上一次对信号捕捉的处理，一般不使用，传递NULL
        返回值：
            成功0，失败-1
    struct sigaction {
    void     (*sa_handler)(int);        //函数指针，指向处理函数
    void     (*sa_sigaction)(int, siginfo_t *, void *); //不常用
    sigset_t   sa_mask; //临时阻塞信号集，在信号捕捉函数执行过程中，临时阻塞某些信号
    int        sa_flags;    //使用哪一个信号处理函数对信号处理
    若为0，使用sa_handler,若为SA_SIGINFO,表示使用sa_sigaction
    void     (*sa_restorer)(void); //已废弃，传NULL即可
    };
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

void myAlarm(int num){
    printf("捕捉到的信号编号是：%d\n",num);
    printf("XXXXXXXXXXXXXX\n");
}
int main(){
    //注册信号捕捉
    //signal(SIGALRM,myAlarm);
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler=myAlarm;
    act.sa_flags=0;
    sigaction(SIGALRM,&act,NULL);
    
    struct itimerval new_value;

    //设置值
    //设置间隔时间
    new_value.it_interval.tv_sec=2;
    new_value.it_interval.tv_usec=0;
    //设置延迟多少秒,3秒时第一次发送信号
    new_value.it_value.tv_sec=3;
    new_value.it_value.tv_usec=0;
    int ret=setitimer(ITIMER_REAL,&new_value,NULL); //非阻塞
    if(ret == -1){
        perror("setitimer");
        return -1;
    }
    printf("定时器开始了...\n");

    //getchar();
    while(1){}
    return 0;
}