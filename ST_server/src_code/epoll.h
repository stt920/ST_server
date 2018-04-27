/*
   Date:2018/4/25
   Author:stt
*/
#ifndef EPOLL_H
#define EPOLL_H
#include <sys/epoll.h>
#include "threadpool.h"


#define MAXEVENTS 1024
#define	MAXEPOLL	10000
#define	MAXLINE		1024



int st_epoll_create(int flags);


int st_epoll_add(int epoll_fd,int fd,int  events);
int st_epoll_del(int epoll_fd,int listen_fd,struct epoll_event* ev);
int st_epoll_wait(int epoll_fd, struct epoll_event* events, int max_events, int timeout);

void do_request(int fd);

int st_handle_events(int epoll_fd,int listen_fd,struct epoll_event* events, \
                        int events_num,st_threadpool_t *tp);

#endif
