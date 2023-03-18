#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>

int main(){
    //创建一个共享内存
    int shmid=shmget(100,4096,IPC_CREAT|0664);

    //和当前进程关联

    void *ptr=shmat(shmid,NULL,0);
    char *str="hello world!";

    //3.写数据

    memcpy(ptr,str,strlen(str)+1);
    printf("按任意键继续...\n");
    getchar();

    //4.写数据
    shmdt(ptr);

    //5.删除共享内存
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}