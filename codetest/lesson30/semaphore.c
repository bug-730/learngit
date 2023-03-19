/*
    信号量的类型sem_t
    #include <semaphore.h>

    int sem_init(sem_t *sem, int pshared, unsigned int value);
        功能：初始化信号量
        参数：
            sem：信号量变量的地址
            pshared:0用在线程间，非0用在线程间
            value：记录信号量中的值
    int sem_destroy(sem_t *sem);
        功能：释放资源
    int sem_wait(sem_t *sem);
        功能：对信号量加锁，信号量的值-1

    int sem_trywait(sem_t *sem);

    int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

    int sem_post(sem_t *sem);
        功能：对信号量解锁，信号量的值加1，同时唤醒被阻塞的wait


*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <semaphore.h>
//创建互斥量
pthread_mutex_t mutex;

//创建信号量
sem_t psem,csem;

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
        //信号量上锁，注意不能放在互斥锁内，会形成死锁
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        struct Node *cur=(struct Node *)malloc(sizeof(struct Node));
        cur->next=Head;
        Head=cur;
        cur->val=rand() %1000;
        printf("add Node, num :%d,tid :%ld\n",cur->val,pthread_self());
        pthread_mutex_unlock(&mutex);
        //信号量解锁
        sem_post(&csem);
        usleep(100);
    }
}

//消费者线程
void *customer(void *args){
    while(1){
        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        //先保留头节点以便释放
        struct Node *temp=Head;   
        Head=Head->next;
        printf("delete Node, num :%d,tid %ld\n",temp->val,pthread_self());
        free(temp);
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        usleep(100);
    }
}


int main(){
    
    //互斥量初始化
    pthread_mutex_init(&mutex,NULL);

    //初始化信号量
    sem_init(&psem,0,8);
    sem_init(&csem,0,0);

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


    //锁释放
    pthread_mutex_destroy(&mutex);

    //主线程退出
    pthread_exit(NULL);
    return 0;
}