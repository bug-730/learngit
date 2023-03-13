/*
    创建fifo文件
    1.命令mkfifo + 名字
    2.

    #include <sys/types.h>
    #include <sys/stat.h>

    int mkfifo(const char *pathname, mode_t mode);
        path：文件路径
        mode：文件的权限和open的mode一样 （& umask）
        返回值：成功返回0， 失败返回-1，设置errno

*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    int ret=mkfifo("fifo1",0664);
    if(ret == -1){
        perror("mkfifo");
        exit(0);
    }
    return 0;
}