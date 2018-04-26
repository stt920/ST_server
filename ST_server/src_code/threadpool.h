/*
   Date:2018/4/26
   Author:stt
*/
#ifndef THREADPOLL_H
#define THREADPOLL_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

typedef struct st_task{
	void (*func)(void *);    // 处理函数的函数指针
	void *arg;    // 函数变量
	struct st_task *next;    // 任务链表（下一节点指针）
}st_task_t;

typedef struct threadpool{
    pthread_mutex_t lock;     // 互斥锁
    pthread_cond_t cond;      // 条件变量
    pthread_t *threads;       // 线程数组       pthread_t线程ID
    st_task_t *head;          // 任务链表
    int thread_count;         // 线程数
    int queue_size;           // 任务链表长
    int shutdown;             // 关机模式
    int started;
}st_threadpool_t;


st_threadpool_t* threadpool_init(int thread_num);
int threadpool_add(st_threadpool_t* pool, void (*func)(void *), void* arg);
int threadpool_destroy(st_threadpool_t* pool, int gracegul);


#endif
