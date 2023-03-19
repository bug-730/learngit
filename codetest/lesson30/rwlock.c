/*
    #include <pthread.h>
    类型 pthread_rwlock_t
    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
              const pthread_rwlockattr_t *restrict attr);
    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

    案例：创建8个线程，同时操作一个全局变量
    其中三个线程不定时写变量，另五个不定时读



*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

long int num = 100;
pthread_rwlock_t rwlock;
//写线程
void *writeT(void *args){
    pthread_rwlock_wrlock(&rwlock);
    num++;
    printf("write process :num++ = %ld,tid = %ld\n",num,pthread_self());
    pthread_rwlock_unlock(&rwlock);
}
//读线程
void *readT(void *args){
    pthread_rwlock_rdlock(&rwlock);
    printf("read process :num = %ld,tid = %ld\n",num,pthread_self());
    pthread_rwlock_unlock(&rwlock);
}
int main(){
    //初始化rw锁
    pthread_rwlock_init(&rwlock,NULL);
    //创建8个线程
    pthread_t wtid[3],rtid[5];
    for(int i=0;i<3;i++){
        pthread_create(&wtid[i],NULL,writeT,NULL);
    }
    for(int i=0;i<5;i++){
        pthread_create(&rtid[i],NULL,readT,NULL);
    }

    //设置线程分离
    for(int i=0;i<3;i++){
        pthread_detach(wtid[i]);
    }
    for(int i=0;i<5;i++){
        pthread_detach(rtid[i]);
    }
    
    //释放锁资源
    pthread_rwlock_destroy(&rwlock);

    //主线程退出
    pthread_exit(NULL);
    return 0;
}
