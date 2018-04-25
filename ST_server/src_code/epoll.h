/*
   Date:2018/4/25
   Author:stt
*/
#ifndef EPOLL_H
#define EPOLL_H
#include <sys/epoll.h>

#define MAXEVENTS 1024
struct epoll_event events[MAXEVENTS];



int st_epoll_creat(int flags);

int st_epoll_add(int epoll_fd,int listen_fd,struct epoll_event* ev);
int st_epoll_del(int epoll_fd,int listen_fd,struct epoll_event* ev);
//int st_epoll_mod(int epoll_fd,int listen_fd,struct epoll_event* ev);


int st_epoll_wait(int epoll_fd, struct epoll_event* events, int max_events, int timeout);



#endif
