/*************************************************************************
	> File Name: epoll_client.c
	> Author: 
	> Mail: 
	> Created Time: Wed 15 Aug 2018 07:08:12 AM PDT
 ************************************************************************/

#include "pub.h"
#include <sys/epoll.h>

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        printf("%s <ip> <port>",argv[0]);
        return -1;
    }
    int st;
    int port = atoi(argv[2]);
    char *ipaddr = "";

    strcpy(ipaddr, argv[1]);
    st = connect_server(ipaddr, port);
    
    socket_send(st);

    close_socket(st);
    
    return 0;
}
