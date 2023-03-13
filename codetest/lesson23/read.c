/*
读数据
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(){
    
    //1.打开管道文件
    int fd = open("test",O_RDONLY);
    if(fd == -1){
        perror("open");
        exit(0);
    }
    //读数据
    char buf[1024]={0};
    while(1){
        memset(buf,0,sizeof(buf));
        int len=read(fd,buf,sizeof(buf));
        if(len == 0){
            printf("写端断开连接...\n");
            break;
        }
        printf("recv buf:%s\n",buf);
    }

    close(fd);
    
    return 0;

}