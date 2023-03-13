#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/*
    设置管道非阻塞
    int flag=fcntl(fd[0],F_GETFL);//获取文件状态flag
    flags|=NOBLOCK;  //修改flag值
    fcntl(fd[0],F_SETFL,flag);//设置新的flag
*/
int main(){
    //创建一个管道，在fork之前
    int pipefd[2];
    int ret=pipe(pipefd);
    if(ret==-1){
        perror("pipe");
        exit(0);
    }
    pid_t pid=fork();

    if(pid > 0){
        //父进程
        printf("i am parent process, pid :%d\n",getpid());
        char buf[1024]={0};
        char *str="hello i am parent!";
        //父进程关闭写端
        close(pipefd[1]);
        int flags=fcntl(pipefd[0],F_GETFL);//获取文件状态flag
        flags|=O_NONBLOCK;  //修改flag值
        fcntl(pipefd[0],F_SETFL,flags);//设置新的flag
        while(1){
            //写
            // write(pipefd[1],str,strlen(str));
            // sleep(1);
            //读
            int len = read(pipefd[0],buf,sizeof(buf));
            printf("len = %d\n",len);
            printf("parent recv buf:%s\n",buf);
            memset(buf,0,sizeof(buf));
            sleep(1);
        }
    }

    else if(pid == 0){
        //子进程
        printf("i am child process, pid :%d\n",getpid());
        char buf[1024]={0};
        char *str="hello i am child!\n";
        //子进程关闭读端
        close(pipefd[0]);
        while(1){
            //读
            // int len = read(pipefd[0],buf,sizeof(buf));
            // printf("child recv buf:%s\n",buf);
            // bzero(buf,1024);
            //写
            write(pipefd[1],str,strlen(str));
            sleep(10);
        }
       
    }
    return 0;
}