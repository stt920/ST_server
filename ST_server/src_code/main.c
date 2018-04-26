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
#define SERVERPORT  1238

int main(int argc, char *argv[])
{
    int listen_fd=socket_bind_listen(SERVERPORT);
    setnonblocking(listen_fd);
    int epoll_fd=epoll_create(MAXEPOLL);

    struct 	epoll_event	ev;
	struct 	epoll_event	evs[MAXEPOLL];

	st_epoll_add(epoll_fd,listen_fd,&ev);

	int cur_fds=1; //当前就绪fd
	while(1)
	{
        int wait_fds=st_epoll_wait(epoll_fd, evs, cur_fds, -1);
        st_handle_events(epoll_fd,listen_fd, evs,ev,wait_fds);

	}
    close(listen_fd);
    return 0;
}
