//使用文件映射实现拷贝文件

/*
    思路：
    1.对原始文件进行内存映射
    2.创建一个新文件并拓展
    3.新文件映射到内存中
    4.通过内存拷贝第一个文件中的数据到新文件中
    5.释放资源

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <sys/mman.h>

int main()
{
    //1.对原始文件做文件映射
    int fd = open("hello.txt",O_RDWR);
    if(fd == -1){
        perror("open");
        exit(0);
    }
    //获取原始文件大小

    int len = lseek(fd,0,SEEK_END);

    //2.创建一个新文件并拓展

    int fd1 = open("copy.txt",O_RDWR|O_CREAT,0664);
    if(fd1 == -1){
        perror("open");
        exit(0);
    }

    //对新文件进行扩展
    truncate("copy.txt",len);
    //write("fd1"," ",1);
    //分别做内存映射
    void *ptr=mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    void *ptr1=mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0);
    if(ptr==MAP_FAILED||ptr1==MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    //内存拷贝
    memcpy(ptr1,ptr,len);

    //释放资源
    munmap(ptr1,len);
    munmap(ptr,len);

    close(fd1);
    close(fd);
    return 0;
}
