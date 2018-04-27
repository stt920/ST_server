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

void *threadpool_worker(void *arg)
{
    if(arg==NULL)
        return NULL;


    st_threadpool_t *pool=(st_threadpool_t *)arg;
    st_task_t *task;

    while(1)
    {
        //线程池上锁
        pthread_mutex_lock(&(pool->lock));


         // 没有task且未停机则阻塞
        while((pool->queue_size == 0) && !(pool->shutdown))
            pthread_cond_wait(&(pool->cond), &(pool->lock));

        // 立即停机模式、平滑停机且没有未完成任务则退出
        if(pool->shutdown == immediate_shutdown)
            break;
        else if((pool->shutdown == graceful_shutdown) && (pool->queue_size == 0))
            break;



        //获取链表中第一个tast
        task=pool->head->next;
        if(task==NULL)
        {
            pthread_mutex_unlock(&(pool->lock));
            continue;
        }

        //存在task则取走
        pool->head->next=task->next;
        pool->queue_size--;
        pthread_mutex_unlock(&(pool->lock));

        //设置task->func参数
        (*(task->func))(task->arg);
        free(task);
    }
    pool->started--;
    pthread_mutex_unlock(&(pool->lock));
    pthread_exit(NULL);
    return NULL;
}


//释放线程资源
int threadpool_destroy(st_threadpool_t *pool,int graceful)
{
    if(pool == NULL)
        return st_tp_invalid;
    if(pthread_mutex_lock(&(pool->lock)) != 0)
        return st_tp_lock_fail;

    int err = 0;
    do{
        if(pool->shutdown){
            err = st_tp_already_shutdown;
            break;
        }

        pool->shutdown = (graceful) ? graceful_shutdown : immediate_shutdown;

        if(pthread_cond_broadcast(&(pool->cond)) != 0){
            err = st_tp_cond_broadcast;
            break;
        }

        if(pthread_mutex_unlock(&(pool->lock)) != 0){
            err = st_tp_lock_fail;
            break;
        }

        // 回收每个线程资源
        for(int i = 0; i < pool->thread_count; i++){
            if(pthread_join(pool->threads[i], NULL) != 0){
                err = st_tp_thread_fail;
            }
        }
    }while(0);

    if(!err){
        pthread_mutex_destroy(&(pool->lock));
        pthread_cond_destroy(&(pool->cond));
        threadpool_free(pool);
    }
    return err;


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
        if(pthread_create(&(pool->threads[i]),NULL,threadpool_worker,(void*)pool)!=0)
            return NULL;
    }


    return pool;
}


int threadpool_add(st_threadpool_t* pool, void (*func)(void *), void* arg)
{
    //int rc=0;
    int err = 0;
    if(pool == NULL || func == NULL)
        return -1;

    if(pthread_mutex_lock(&(pool->lock)) != 0)
        return -1;

    // 已设置关机
    if(pool->shutdown){
        err = st_tp_already_shutdown;
        goto out;
    }

    // 新建task并注册信息
    st_task_t *task = (st_task_t *)malloc(sizeof(st_task_t));
    if(task == NULL)
        goto out;
    task->func = func;
    task->arg = arg;

    // 新task节点在head处插入
    task->next = pool->head->next;
    pool->head->next = task;
    pool->queue_size++;

    pthread_cond_signal(&(pool->cond));

out:
    if(pthread_mutex_unlock(&pool->lock) != 0)
        return -1;
    return err;

}
