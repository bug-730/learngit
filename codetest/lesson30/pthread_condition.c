/*
    #include <pthread.h>
    
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    
    int pthread_cond_destroy(pthread_cond_t *cond);
    int pthread_cond_init(pthread_cond_t *restrict cond,
              const pthread_condattr_t *restrict attr);
    int pthread_cond_timedwait(pthread_cond_t *restrict cond,
              pthread_mutex_t *restrict mutex,
              const struct timespec *restrict abstime);
        功能：阻塞线程，直到指定的时间结束
    int pthread_cond_wait(pthread_cond_t *restrict cond,
              pthread_mutex_t *restrict mutex);
        功能：阻塞函数，调用该线程，线程会阻塞，等待唤醒
              当阻塞时会将锁释放，当被唤醒时重新加锁
    int pthread_cond_broadcast(pthread_cond_t *cond);
        功能：唤醒所有进程
    int pthread_cond_signal(pthread_cond_t *cond);
        功能：唤醒一个或多个等待的线程


*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

//创建互斥量
pthread_mutex_t mutex;

//创建条件变量
pthread_cond_t cond;

//链表
struct Node{
    int val;
    struct Node *next;
};

//定义链表头
struct Node *Head=NULL;

//生产者线程
void *producer(void *args){
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node *cur=(struct Node *)malloc(sizeof(struct Node));
        cur->next=Head;
        Head=cur;
        cur->val=rand() %1000;
        printf("add Node, num :%d,tid :%ld\n",cur->val,pthread_self());
        //唤醒消费者
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
}

//消费者线程
void *customer(void *args){
    while(1){
        pthread_mutex_lock(&mutex);
        //先保留头节点以便释放
        //判断是否有数据
        while(Head==NULL){
            pthread_cond_wait(&cond,&mutex);
        }
        assert(Head!=NULL);
        struct Node *temp=Head;   
        Head=Head->next;
        printf("delete Node, num :%d,tid %ld\n",temp->val,pthread_self());
        free(temp);
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
}


int main(){
    
    //互斥量初始化
    pthread_mutex_init(&mutex,NULL);

    //初始化条件变量
    pthread_cond_init(&cond,NULL);

    //创建5个生产者，5个消费者
    pthread_t ptids[5],ctids[5];
    for(int i=0;i<5;i++){
        pthread_create(&ptids[i],NULL,producer,NULL);
        pthread_create(&ctids[i],NULL,customer,NULL);
    }
    
    //线程分离
    for(int i=0;i<5;i++){
        pthread_detach(ptids[i]);
        pthread_detach(ctids[i]);
    }
    
    while(1){
        sleep(10);
    }

    //释放条件变量
    pthread_cond_destroy(&cond);

    //锁释放
    pthread_mutex_destroy(&mutex);

    //主线程退出
    pthread_exit(NULL);
    return 0;
}