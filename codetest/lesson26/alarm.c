/*
    #include <unistd.h>

    unsigned int alarm(unsigned int seconds);
        功能：设置定时器，函数调用，开始倒计时，倒计时为0时
        函数会给当前的进程发送一个信号：SIGALARM，该函数不阻塞
        参数：
            seconds：倒计时的时长，单位为秒。如果参数为0，定时器无效，
            取消一个定时器，通过alarm(0)，两次调用后一次覆盖前一次
        返回值：
            之前有定时器，返回之前倒计时剩余时间
            之前没有定时器，返回0
        SIGALARM：默认终止当前的进程，每一个进程都有且只有唯一的一个定时器

*/

#include <stdio.h>
#include <unistd.h>

int main(){

    int seconds=alarm(5);
    printf("seconds=%d\n",seconds); //0
    sleep(3);
    seconds=alarm(5);  //不阻塞
    printf("seconds=%d\n",seconds); //2

    while(1){}
    return 0;
}