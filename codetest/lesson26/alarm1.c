//一秒钟电脑能数多少个数？


/*
    程序运行时间>1s,原因是IO操作耗时
    实际时间=内核时间+用户时间+消耗的时间

    定时器，与进程的状态无关（自然定时法），无论进程处于什么状态，计时器都会计时
*/
#include <stdio.h>
#include <unistd.h>

int main(){
    int i=0;
    alarm(1);
    while(1){
        printf("%d\n",i++);
    }
    return 0;
}