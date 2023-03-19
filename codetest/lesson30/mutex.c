/*
    #include <pthread.h>

    pthread_mutex_t 

    int pthread_mutex_init(pthread_mutex_t *restrict mutex,
              const pthread_mutexattr_t *restrict attr);
        功能：初始化互斥量
        参数：
            mutex：需要初始化的互斥量变量
            attr：互斥量相关的属性，NULL
            （restrict）：C语言的修饰符，被修饰的指针不能有另外的指针进行操作
    int pthread_mutex_destroy(pthread_mutex_t *mutex);
        功能：释放互斥量的资源

    int pthread_mutex_lock(pthread_mutex_t *mutex);
        功能：加锁，阻塞的，如果有一个线程加锁了，那么其他线程只能阻塞
    int pthread_mutex_trylock(pthread_mutex_t *mutex);
        功能：尝试加锁，加锁失败不阻塞，直接返回
    int pthread_mutex_unlock(pthread_mutex_t *mutex);
        功能：解锁

    

*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

unsigned int tickets=1000;
//创建互斥量
pthread_mutex_t mutex;
void *selltickets(void *args){
    
    //卖票
    while(1){
        //加锁
        pthread_mutex_lock(&mutex);
        if(tickets>0){
            printf("%ld 正在卖第 %d张门票\n",pthread_self(),tickets);
            tickets--;
        }else{
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        //解锁
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(){

    //初始化互斥量
    pthread_mutex_init(&mutex,NULL);
    //创建三个子线程
    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1,NULL,selltickets,NULL);
    pthread_create(&tid2,NULL,selltickets,NULL);
    pthread_create(&tid3,NULL,selltickets,NULL);

    //join
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    //或者设置线程分离
    // pthread_detach(tid1);
    // pthread_detach(tid2);
    // pthread_detach(tid3);
    pthread_mutex_destroy(&mutex);
    //退出主线程
    pthread_exit(NULL);
}