/*
    
    标准C库函数
    #include <stdio.h>
    int fseek(FILE *stream, long offset, int whence);
    
    linux库函数
    #include <sys/types.h>
    #include <unistd.h>
    off_t lseek(int fd, off_t offset, int whence);
        参数：
        - offset：偏移量
        - whence；
            SEEK_SET
                设置文件指针的偏移量
            SEEK_CUR
                设置偏移量：当前位置+第二个参数offset的值
            SEEK_END
                设置偏移量：文件大小+第二个参数的值
            返回值：文件指针的位置
        作用：
            1.移动文件指针到文件头
            lseek(fd,0,SEEK_SET);
            2.获取当前文件指针位置
            lseek(fd,0,SEEK_CUR);
            3.获取文件长度
            lseek(fd,0,SEEK_END);
            4.拓展文件长度，当前文件10b，增加100字节
            lseek(fd,100,SEEK_END);
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    
    int fd=open("hello.txt",O_RDWR);

    if(fd==-1){
        perror("open");
        return -1;
    }
    //扩展文件的长度
    int ret=lseek(fd,100,SEEK_END);
    if(ret==-1){
        perror("lseek");
        return -1;
    }
    write(fd," ",1);
    return 0;
}