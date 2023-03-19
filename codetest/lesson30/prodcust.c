/*
    生产者消费者模型（粗略版本）
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//创建互斥量
pthread_mutex_t mutex;

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
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
}

//消费者线程
void *customer(void *args){
    while(1){
        pthread_mutex_lock(&mutex);
        //先保留头节点以便释放
        struct Node *temp=Head;
        //判断是否有数据
        if(temp!=NULL){
            Head=Head->next;
            printf("delete Node, num :%d,tid %ld\n",temp->val,pthread_self());
            free(temp);
            pthread_mutex_unlock(&mutex);
        }else{
            printf("No product...\n");
            pthread_mutex_unlock(&mutex);
        }
        usleep(100);
    }
}


int main(){
    
    //互斥量初始化
    pthread_mutex_init(&mutex,NULL);

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