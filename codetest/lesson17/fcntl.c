/*
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd, ...  );
    cmd：表示对文件描述符的操作方式
        F_DUPFD：复制fd，返回新的描述符（dup函数）
        F_GETFL：获取文件状态flag（获取的就是open函数传递的flag）
        F_SETFL：设置文件描述符文件状态flag
         必选项：
         可选项：O_APPEND表示追加数据
                NONBLOCK设置成非阻塞
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
int main(){
    //返回新的描述符
    //int fd=open("1.txt",O_RDONLY|O_CREAT,0664);
    //int fd=fcntl(fd,F_DUPFD);

    int fd=open("1.txt",O_RDWR|O_CREAT,0664);
    if(fd==-1){
        perror("open");
        return -1;
    }

    int flag=fcntl(fd,F_GETFL);
    flag|=O_APPEND;
    int ret=fcntl(fd,F_SETFL,flag);
    if(ret==-1){
        perror("fcntl");
        return -1;
    }

    const char *str="hello nihao";
    write(fd,str,strlen(str));

    close(fd);
    return 0;
}