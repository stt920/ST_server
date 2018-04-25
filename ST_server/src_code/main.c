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
#define MAXLINE 80  //buf缓冲区大小
#define SERVERPORT  7871

#define	MAXEPOLL	10000

int main(int argc, char *argv[])
{
    char* buf={0};
    int listen_fd=socket_bind_listen(SERVERPORT);

    int epoll_fd=st_epoll_creat(MAXEPOLL);
    struct 	epoll_event	ev;
	struct 	epoll_event	evs[MAXEPOLL];

	st_epoll_add(epoll_fd,listen_fd,&ev);


	int cur_fds=1; //当前就绪fd
	while(1)
	{
        int st_epoll_wait(epoll_fd, evs, cur_fds, -1);

	}


 /*
   while(1)
   {
   int accept_fd=Accept(listen_fd);

    if(fork()==0)
    {
        while(1)
        {
            int n=read(accept_fd,buf,MAXLINE);
            if(n>0)
            printf("received data:%s",buf);
        }
    }
   }
   */
    close(listen_fd);
    return 0;
}
