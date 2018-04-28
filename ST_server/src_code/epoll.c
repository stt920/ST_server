/*
   Date:2018/4/25
   Author:stt
*/
#include "epoll.h"
#include "Socket.h"
#include <sys/epoll.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
//创建epoll文件描述符
int st_epoll_create(int flags)
{
    int epoll_fd = epoll_create(flags);
    if(epoll_fd == -1)
        printf("st_epoll_create Error : %d\n", errno);
    return epoll_fd;
}

//epoll_ctl相关封装
int st_epoll_add(int epoll_fd,int fd,int  events)
{
    struct epoll_event event;
    event.data.fd=fd;
    event.events=events;
    int ret=epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
	if(ret==-1)
        printf("st_epoll_add Error : %d\n", errno);
    return 1;
}

int st_epoll_del(int epoll_fd,int fd,struct epoll_event* ev)
{
    close(ev->data.fd);
    int ret=epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd, ev);
    if(ret==-1)
    {
        printf("st_epoll_del Error : %d\n", errno);
    }

    return 1;
}

// 返回活跃事件数
int st_epoll_wait(int epoll_fd, struct epoll_event* events, int max_events, int timeout)
{
    int ret_count = epoll_wait(epoll_fd, events, max_events, timeout);       //!!!!!未知bug   2018.04.27.17：40
    if(ret_count==-1)
    {
        printf("st_epoll_wait Error : %d\n", errno);
    }

    return ret_count;
}
//事件分发处理
int st_handle_events(int epoll_fd,int listen_fd,struct epoll_event* events, \
                        int events_num,st_threadpool_t *tp)
{

    for(int i=0;i<events_num;i++)
    {
        if(events[i].data.fd==listen_fd)
        {

            int conn_fd=Accept_epolladd(listen_fd,epoll_fd);
            if(conn_fd<0)
                printf("st_handle_events ERROR1: %d\n", errno);
            continue;
        }
        else
        {
            //排除错误的事件
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
                || (!(events[i].events & EPOLLIN))){
                close(events[i].data.fd);
                continue;
            }

        }
        //将任务加入线程池

        threadpool_add(tp,do_request,&events[i].data.fd);

    }

    return 0;
}

/*
int st_handle_events(int epoll_fd,int listen_fd,struct epoll_event* evs,struct epoll_event ev,int events_num)
{
    int cur_fds=1;
    int nread=0;
    char buf[MAXLINE];
    for(int i=0;i<events_num;i++)
    {
            if(evs[i].data.fd==listen_fd&&cur_fds<MAXEPOLL)
            {
                int conn_fd=Accept(listen_fd,epoll_fd);
                if(conn_fd<0)
                    printf("st_handle_events ERROR1: %d\n", errno);
                st_epoll_add(epoll_fd,conn_fd,EPOLLIN);
				++cur_fds;
                continue;
            }
            nread = read( evs[i].data.fd, buf, sizeof( buf ) );
            if(nread<=0)
            {
                close(evs[i].data.fd);
                st_epoll_del(epoll_fd,evs[i].data.fd,&ev);
                --cur_fds;
                continue;
            }
            printf("rec date:");
            for(int i=0;i<nread;i++)
                printf("%c",buf[i]);
            printf("\n");
    }
    return 1;
}*/
//test 服务器打印客户端输入
void do_request(void *ptr)
{

    int* fd=(int *)ptr;
    char buf[MAXLINE];
    printf("rcv date:");


    int nread = read(*fd, buf, sizeof( buf ) );

    if(nread>0)
    {
        for(int i=0;i<nread;i++)
            printf("%c",buf[i]);
        printf("\n");
    }

}
