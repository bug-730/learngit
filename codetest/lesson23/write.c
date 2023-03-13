//向管道写数据

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/*
有名管道总结：
    1.一个为只读打开一个管道文件的进程会阻塞，直到另外一个进程为只写打开管道
    2.一个为只写打开一个管道文件的进程会阻塞，直到另外一个进程为只读打开管道

    读管道：
        管道中有数据：read返回读到的字节数
        管道中无数据：
            写端全部关闭：read返回0.（相当于读到文件末尾）
            写端未全部关闭：read阻塞
    写管道：
        读端全部关闭：进程异常终止（收到SIGPIPE信号）
        读端没有全部关闭：
            管道满，阻塞等待
            未满向里写数据，并返回实际收到的字节数

*/

int main(){
    int ret=access("test",F_OK); //判断管道文件是否存在
    if(ret == -1){
        printf("管道不存在，创建管道\n");
        //创建管道
        ret=mkfifo("test",0664);
        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
    }

    //打开管道文件(只写)
    int fd = open("test",O_WRONLY);
    if(fd == -1){
        perror("open");
        exit(0);
    }
    char buf[1024]={0};
    for(int i=0;i<100;i++){
        memset(buf,0,sizeof(buf));
        sprintf(buf,"Hello:%d",i);
        printf("write data:%s\n",buf);
        write(fd,buf,strlen(buf));
        sleep(1);
    }

    close(fd);

    return 0;
}