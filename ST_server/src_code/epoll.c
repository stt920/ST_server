/*
   Date:2018/4/25
   Author:stt
*/

#include "epoll.h"
#include <sys/epoll.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


//创建epoll文件描述符
int st_epoll_create(int flags){
    int epoll_fd = epoll_create1(flags);
    if(epoll_fd == -1)
        return -1;
    return epoll_fd;
}

//epoll_ctl相关封装
int st_epoll_add(int epoll_fd,int listen_fd,struct epoll_event* ev)
{
    ev->data.fd=listen_fd;
    ev->events=EPOLLIN|EPOLLET;
    int ret=epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, ev );
	if(ret==-1)
        return -1;
    return 0;
}

int st_epoll_del(int epoll_fd,int listen_fd,struct epoll_event* ev)
{
    close(ev->data.fd);
    int ret=epoll_ctl(epoll_fd,EPOLL_CTL_DEL,listen_fd, ev);
    if(ret==-1)
        return -1;
    return 0;
}






// 返回活跃事件数
int st_epoll_wait(int epoll_fd, struct epoll_event* events, int max_events, int timeout)
{
    int ret_count = epoll_wait(epoll_fd, events, max_events, timeout);
    return ret_count;
}

