/*
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>

    int stat(const char *pathname, struct stat *buf);
    作用：获取一个文件相关的一些信息
    参数：
        - pathname:操作的文件路径
        - statbuf:结构体变量，传出参数，用于保存获取到的文件的信息
    返回值：
        成功返回0
        失败返回-1并设置errno
    int lstat(const char *pathname, struct stat *buf);
    作用：获取软连接文件的信息
    其他同上
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    //创建结构体
    struct stat info;
    //stat函数将信息保存在结构体中
    int res=stat("a.txt",&info);
    if(res==-1){
        perror("stat");
        return -1;
    }
    //输出
    printf("size:%ld\n",info.st_size);
    return 0;
}