/*
#include <unistd.h>

int dup(int oldfd);
    作用：复制一个新的文件描述符
    从空闲的文件描述符表中找一个最小的，也指向当前fd指向的文件
*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main(){
    int fd=open("a.txt",O_CREAT| O_RDWR,0664);
    int fd1=dup(fd);
    if(fd==-1||fd1==-1){
        perror("open/dup");
        return -1;
    }
    printf("fd:%d,fd1:%d",fd,fd1);
    fflush(stdout);
    close(fd);
    const char *tex="Hello";
    int ret=write(fd1,tex,strlen(tex));
    if(ret==-1){
        perror("write");
        return -1;
    }
    while(1){}
    close(fd1);
    return 0;
}