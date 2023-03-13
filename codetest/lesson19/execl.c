/*
#include <unistd.h>

extern char **environ;

int execl(const char *path, const char *arg, ...);

int execlp(const char *file, const char *arg, ...);
    path:需要指定执行的文件路径与名称(execlp函数只需指定名称，它会去环境变量中查找)
    arg：执行可执行文件所需要的参数列表（第一个参数一般写程序名称）
        参数最后需要以NULL结束
    只有当调用失败时有返回值-1，调用成功无返回值
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    pid_t pid=fork();
    if(pid == -1){
        perror("fork");
        return -1;
    }
    if(pid>0){
        //父进程
        printf("i am parent process\n");
        sleep(1);
    }
    else if(pid == 0){
        //子进程
        //execl("hello","hello",NULL);
        execlp("ps","ps","aux",NULL);
        printf("i am child process\n");
    }
    for(int i=0;i<4;i++){
        printf("i = %d, pid = %d\n",i,getpid());
    }

    return 0;
}