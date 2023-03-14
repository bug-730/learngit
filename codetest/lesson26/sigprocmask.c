/*
    #include <signal.h>
    int sigprocmask(int how, const sigset_t *restrict set,
        sigset_t *restrict oset);
        功能：将自定义信号集中的数据设置到内核中（设置阻塞，解除阻塞，替换）
        参数：
            how：如何对内核阻塞信号集进行处理
                SIG_BLOCK:将用户设置的阻塞信号集添加到内核中，相当于异或
                SIG_UNBLOCK:根据用户设置的数据，对内核中的数据进行解除操作
                mask &= ~set
                SIG_SETMASK:覆盖掉内核中的值
            set:用户自己设置的信号集
            oldset:保存之前内核中的阻塞信号集的状态，可以是NULL
        返回值：
            成功返回0
            失败返回-1，并设置错误号：
            EFAULT，EINVAL
    int sigpending(sigset_t *set)
        功能：获取内核中的未决信号集
        参数：set，传出参数，保存的是内核中未决信号集中的信息
        返回值：成功0，失败-1

*/

//编写一个程序，将所有常规信号（1-31）的未决状态打印到屏幕
//设置某些信号是阻塞的，通过键盘产生这些信号
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

int main(){
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGQUIT);

    //修改内核中的阻塞信号集
    sigprocmask(SIG_BLOCK,&set,NULL);
    int num = 0;
    while(1){

        //获取当前的未决信号集的数据
        sigset_t sigpend;
        sigemptyset(&sigpend);
        assert(sigpending(&sigpend)==0);
            for(int i=1;i<=31;i++){
                int ret = sigismember(&sigpend,i);
                if(ret == 1){
                    printf("1");
                }else if(ret == 0){
                    printf("0");
                }
                else{
                    perror("sigismember");
                    return -1;
                }
            }
            printf("\n");
            sleep(1);
            num++;
            //解除阻塞
            if(num == 10){
                sigprocmask(SIG_UNBLOCK,&set,NULL);
            }
        }
        return 0;
}
