/*
   Date:2018/4/24
   Author:stt
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Socket.h"
#define MAXLINE 80  //buf缓冲区大小
#define SERVERPORT  22355
int main(int argc, char *argv[])  
{
   char buf[MAXLINE]={0};
   int listen_fd=socket_bind_listen(SERVERPORT);
   
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
    close(listen_fd);
    return 0;
}
