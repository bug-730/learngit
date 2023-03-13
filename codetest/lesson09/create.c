/*
#includ e <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

       int open(const char *pathname, int flags);
       
       
       int open(const char *pathname, int flags, mode_t mode)
       -flags:
        - 必选项：
        - 可选项： O_CREATE没有文件则创建
       -modes:表示创建出的新的文件的操作权限，比如：0775
       int creat(const char *pathname, mode_t mode);



*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    int fd=open("create.txt",O_RDWR | O_CREAT,0777);

    if(fd==-1){
        perror("open");
    }

    close(fd);
    return 0;
}