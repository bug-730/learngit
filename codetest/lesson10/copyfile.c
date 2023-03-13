/*
#include <unistd.h>

    ssize_t read(int fd, void *buf, size_t count);
    - fd
        - open函数返回的文件描述符
    - buf
        - 要读的数据缓冲区（传出参数）
    - count
        - 指定的数组大小
    成功返回读取的字节数，若读到文件末尾则返回0
    失败返回-1，并设置errno
    ssize_t write(int fd, const void *buf, size_t count);
    参数如上
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    
    //open打开文件
    int srcfd=open("english.txt",O_RDONLY);
    if(srcfd==-1){
        perror("open");
        return -1;
    }
    //创建新文件（拷贝文件）
    int destfd=open("cpy.txt",O_WRONLY|O_CREAT,0664);
    if(srcfd==-1){
        perror("open");
        return -1;
    }
    //读写文件
    char buf[1024]={0};
    int len=0;
    while((len=read(srcfd,buf,sizeof(buf)))>0){
        write(destfd,buf,len);
    }


    //关闭文件
    close(srcfd);
    close(destfd);
    return 0;
}