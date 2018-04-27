/*
   Date:2018/4/24
   Author:stt
*/
#ifndef SOCKET_H
#define SOCKET_H

#define LISTENS 1024

int socket_bind_listen(int port);
int setnonblocking(int fd);
int Accept_epolladd(int listen_fd,int epoll_fd);

#endif
