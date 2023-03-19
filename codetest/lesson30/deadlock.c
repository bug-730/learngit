#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
//互斥锁定义
pthread_mutex_t mutex1,mutex2;

void *workA(void *){
    //先加mutex1
    pthread_mutex_lock(&mutex1);
    sleep(1);
    pthread_mutex_lock(&mutex2);
    printf("WorkA...\n");
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}

void *workB(void *){
    //先加mutex2
    pthread_mutex_lock(&mutex2);
    sleep(1);
    pthread_mutex_lock(&mutex1);
    printf("WorkA...\n");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
}

int main(){
    //初始化锁
    pthread_mutex_init(&mutex1,NULL);
    pthread_mutex_init(&mutex2,NULL);
    
    //创建两个线程
    pthread_t th1,th2;
    pthread_create(&th1,NULL,workA,NULL);
    pthread_create(&th2,NULL,workB,NULL);
    
    //回收线程资源
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);

    //释放锁资源
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    return 0;
}