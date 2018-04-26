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
int st_epoll_create(int flags){
    int epoll_fd = epoll_create1(flags);
    if(epoll_fd == -1)
        printf("st_epoll_create Error : %d\n", errno);
    return epoll_fd;
}

//epoll_ctl相关封装
int st_epoll_add(int epoll_fd,int fd,struct epoll_event* ev)
{
    ev->data.fd=fd;
    ev->events=EPOLLIN|EPOLLET;
    int ret=epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, ev );
	if(ret==-1)
        printf("st_epoll_add Error : %d\n", errno);
    return 1;
}

int st_epoll_del(int epoll_fd,int fd,struct epoll_event* ev)
{
    close(ev->data.fd);
    int ret=epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd, ev);
    if(ret==-1)
        printf("st_epoll_del Error : %d\n", errno);
    return 1;
}

// 返回活跃事件数
int st_epoll_wait(int epoll_fd, struct epoll_event* events, int max_events, int timeout)
{
    int ret_count = epoll_wait(epoll_fd, events, max_events, timeout);
    return ret_count;
}
//事件分发处理
int st_handle_events(int epoll_fd,int listen_fd,struct epoll_event* evs,struct epoll_event ev,int events_num/*st_epoll_wait的返回值，但前就绪打任务*/)
{
    int cur_fds=1;
    int nread=0;
    char buf[MAXLINE];
    for(int i=0;i<events_num;i++)
    {
            if(evs[i].data.fd==listen_fd&&cur_fds<MAXEPOLL)
            {
                int conn_fd=Accept(listen_fd);
                if(conn_fd<0)
                    printf("st_handle_events ERROR1: %d\n", errno);
                st_epoll_add(epoll_fd,conn_fd,&ev);
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
}
