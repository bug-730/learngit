#include <string.h>
#include <unistd.h>
#include <stdio.h>
int main(){
    int pipefd[2]={0};

    int ret=pipe(pipefd);
    long size = fpathconf(pipefd[0],_PC_PIPE_BUF);
    printf("pipe size:%d\n",size);
    return 0;
}