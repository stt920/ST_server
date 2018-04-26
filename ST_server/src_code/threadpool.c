/*
   Date:2018/4/26
   Author:stt
*/
#include "threadpool.h"
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
//释放threadpool
int threadpool_free(st_threadpool_t *pool)
{
    if(pool==NULL||pool->started>0)
        return -1;
    //释放线程数组
    if(pool->threads)
        free(pool->threads);

    //销毁任务链表
    st_task_t *old;
    while(pool->head->next)
    {
        old=pool->head->next;
        pool->head->next=pool->head->next->next;
        free(old);
    }
    return 0;
}
void *threadpool_work(void *arg)
{

    return NULL;
}

st_threadpool_t* threadpool_init(int thread_num)
{
    st_threadpool_t* pool;
    pool=(st_threadpool_t*)malloc(sizeof(st_threadpool_t));
    if(pool==NULL)
        printf("threadpool_init1 Error : %d\n", errno);

    // threads指针指向线程数组（存放tid），数组大小即为线程数
    pool->thread_count=0;
    pool->queue_size=0;
    pool->shutdown=0;
    pool->started=0;

    pool->threads=(pthread_t*)malloc(sizeof(pthread_t)*thread_num);//分配线程数组

    //初始化头节点
    pool->head=(st_task_t*)malloc(sizeof(st_task_t));
    pool->head->func=NULL;
    pool->head->arg=NULL;
    pool->head->next=NULL;

    //初始化锁
    pthread_mutex_init(&(pool->lock),NULL);


    //创建线程 thread_num个
    for(int i=0;i<thread_num;i++)
    {
        if(pthread_create(&(pool->threads[i]),NULL,threadpool_work,(void*)pool)!=0)
            return NULL;
    }


    return pool;
}
