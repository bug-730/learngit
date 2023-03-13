/*

    #include <unistd.h>
    void _exit(int status);

    #include <stdlib.h>
    void _Exit(int status);
    status:进程退出时的一个状态信息，父进程回收子进程资源时可以获得
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(){

    printf("Hello\n");
    printf("world");
    _exit(0);
    return 0;
}