/*
   Date:2018/4/24
   Author:stt
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Socket.h"
#include "epoll.h"
#include "errno.h"
#include "threadpool.h"


#define SERVERPORT  1251

struct epoll_event *events;

int main(int argc, char *argv[])
{

    int listen_fd=socket_bind_listen(SERVERPORT);

    setnonblocking(listen_fd);

    int epoll_fd=st_epoll_create(0);

    st_threadpool_t *tp=threadpool_init(4);


    int ep=st_epoll_add(epoll_fd,listen_fd,(EPOLLIN | EPOLLET));
    printf("st_epoll_add:%d\n",ep);

    if(tp!=NULL)
        printf("server start\n");

    while(1)
    {

        int events_num=st_epoll_wait(epoll_fd,events ,MAXEVENTS,-1);

        st_handle_events(epoll_fd,listen_fd,events,events_num,tp);
    }

    close(listen_fd);
    return 0;
}
