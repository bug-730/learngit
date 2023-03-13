/*
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);
    作用：重定向文件描述符
    将newfd指向的文件close，newfd指向oldfd指向的文件
*/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main(){

    int fd1=open("1.txt",O_RDWR|O_CREAT,0664);
    if(fd1==-1){
        perror("open");
        return -1;
    }
    int fd2=open("2.txt",O_RDWR|O_CREAT,0664);
    if(fd2==-1){
        perror("open");
        return -1;
    }

    printf("fd1:%d\tfd2:%d\n",fd1,fd2);
    fflush(stdout);
    int fd3=dup2(fd1,fd2);
    if(fd3==-1){
        perror("dup2");
        return -1;
    }
    const char *str="hello dup2";
    int ret=write(fd2,str,strlen(str));
    if(ret==-1){
        perror("write");
        return -1;
    }
    printf("fd1:%d\tfd2:%d\tfd3:%d\n",fd1,fd2,fd3);

    while(1){}
    return 0;
}