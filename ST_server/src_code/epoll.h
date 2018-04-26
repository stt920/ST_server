/*
   Date:2018/4/25
   Author:stt
*/
#ifndef EPOLL_H
#define EPOLL_H
#include <sys/epoll.h>

#define MAXEVENTS 1024
#define	MAXEPOLL	10000
#define	MAXLINE		1024

struct epoll_event events[MAXEVENTS];



int st_epoll_creat(int flags);

int st_epoll_add(int epoll_fd,int listen_fd,struct epoll_event* ev);
int st_epoll_del(int epoll_fd,int listen_fd,struct epoll_event* ev);
int st_epoll_wait(int epoll_fd, struct epoll_event* events, int max_events, int timeout);

int st_handle_events(int epoll_fd,int listen_fd,struct epoll_event* evs,struct epoll_event ev,int events_num/*st_epoll_wait的返回值，但前就绪打任务*/);



#endif
