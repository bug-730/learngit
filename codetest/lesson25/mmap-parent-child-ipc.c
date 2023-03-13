/*
#include <sys/mman.h>

       void *mmap(void *addr, size_t length, int prot, int flags,
                    int fd, off_t offset);
            功能：将一个文件或者设备映射到内存中
            参数：
                void *addr:NULL,由内核指定
                length:要映射的数据长度，这个值不能为0，建议使用文件的长度（lseek，stat）
                prot:对申请的内存映射区的权限：
                    PROT_EXEC：可执行

                    PROT_READ：读权限

                    PROT_WRITE：写权限

                    PROT_NONE：没有权限
                    要操作映射内存，必须要有读的权限
                    PROT_READ、PROT_READ|PROT_WRITE
                flags：
                    MAP_SHARED：映射区的数据会自动和磁盘文件同步，进程间通信必须设置这个选项
                    MAP_PRIVATE：不同步，会重新创建一个新文件
                fd：文件的文件描述符
                    通过open得到，open的是一个磁盘文件
                    文件大小不能为零，且open权限不能和prot参数有冲突（prot权限需要小于等于open）
                offset：偏移量，一般不用。必须指定的是4k的整数倍，0表示不偏移
            返回值：返回创建的内存首地址
                失败返回MAP_FAILED,(void *) -1

       int munmap(void *addr, size_t length);
            功能：释放内存映射
            参数：
                addr：要释放的内存首地址
                length：要释放的内存大小，要和mmap中的length大小一样

*/

/*
    使用内存映射实现进程间通信：
    有关系的进程（父子进程）
        还没有子进程的时候
            通过唯一的父进程，先创建内存映射区
        有了内存映射区，创建子进程
        父子进程共享创建的内存映射区
    没有关系的进程间通信
        准备一个大小不是0的磁盘文件
        进程1 通过磁盘文件创建内存映射区
            得到一个操作这块内存的指针
        进程2 通过磁盘文件创建内存映射区
            得到一个操作这块内存的指针
        使用内存映射区通信

        注意：内存映射区通信，非阻塞
*/

#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(){
    //打开文件
    int fd=open("hello.txt",O_RDWR);

    int size = lseek(fd,0,SEEK_END); //获取文件大小

    //获取文件映射区

    void *ptr = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }


    //创建子进程
    pid_t pid=fork();
    if(pid > 0){
        //父进程
        wait(NULL);
        char buf[128];
        strcpy(buf,(char *)ptr);
        printf("buf recv:%s\n",buf);
    }
    else if(pid == 0){
        //子进程
        sleep(3);
        strcpy((char *)ptr,"nihao a,father!!!!");
    }


    //关闭内存映射区
    munmap(ptr,size);
    return 0;
}