/*
       #include <sys/time.h>

       
       int setitimer(int which, const struct itimerval *new_value,
                     struct itimerval *old_value);
            功能：设置定时器，可以替代alarm函数，精度微妙，可以实现周期性定时
            参数：
                which：定时器以什么时间计时
                ITIMER_REAL:真实时间，时间到达，发送SIGALRM 常用
                ITIMER_VIRTUAL:用户时间，时间到达，发送SIGVTALRM
                ITIMER_PROF:用户态+内核态，时间到达，发送SIGPROF
            new_value:设置定时器属性
            struct itimerval {  //定时器结构体
            struct timeval it_interval; //每个阶段的时间
            struct timeval it_value;    //延迟多长时间执行定时器
            };

            struct timeval {        //时间结构体
               time_t      tv_sec;         //秒数
               suseconds_t tv_usec;        //微秒
            };
        过十秒后，每隔两秒执行一次
            old_value：记录上一次定时的时间参数，一般不使用，指定NULL
        返回值：
            成功返回0，失败返回-1，并设置错误号

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

int main(){
    struct itimerval new_value;

    //设置值
    //设置间隔时间
    new_value.it_interval.tv_sec=2;
    new_value.it_interval.tv_usec=0;
    //设置延迟多少秒
    new_value.it_value.tv_sec=3;
    new_value.it_value.tv_usec=0;
    int ret=setitimer(ITIMER_REAL,&new_value,NULL); //非阻塞
    if(ret == -1){
        perror("setitimer");
        return -1;
    }
    printf("定时器开始了...\n");

    getchar();
    return 0;
}