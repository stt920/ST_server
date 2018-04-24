/*
   Date:2018/4/24
   Author:stt
*/

#include "Socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
//设置非阻塞
/*
int setnonblocking(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1)
        return -1;

    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1)
        return -1;   
}
*/


int socket_bind_listen(int port)
{
    //创建socket
    int listen_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd==-1)
    {
        //close(listen_fd);
        return -1;
    }
    
    //设置服务器ip、port,并与listen_fd绑定
    struct sockaddr_in server_addr;
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    
    if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        return -1;
       
    // 开始监听，最大等待队列长为LISTENS=1024
    if(listen(listen_fd, LISTENS) == -1)
        return -1;
        
    return listen_fd;   
}

int Accept(int listen_fd)
{
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len=sizeof(client_addr) ;
    int accept_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(accept_fd == -1)
        perror("accept");
    return accept_fd;
}













