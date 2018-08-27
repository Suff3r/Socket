/*************************************************************************
	> File Name: epoll_elect_server.c
	> Author: 
	> Mail: 
	> Created Time: Mon 27 Aug 2018 12:12:31 AM PDT
 ************************************************************************/

#include<stdio.h>
#include"pub.h"

#define MAXSOCKET 1024

int main(int argc, const char* argv[])
{
    int serv_fd;
    int i,maxfd;
    int client[MAXSOCKET];
    fd_set allset;
    // wait for socket receive event
    int rc;

    if (argc < 2)
    {
        printf("Input like %s <port>\n", argv[0]);
        return 0;
    }
    serv_fd = server_socket(atoi(argv[1]));
    if (serv_fd == -1)
    {
        return -1;
    }
    //set no_blocking socket
    setnonblock(serv_fd);
    
    for(i = 0; i < MAXSOCKET; i++)
    {
        client[i] = -1;
    }
    maxfd = serv_fd;

    while(1)
    {
        FD_ZERO(&allset);
        FD_SET(serv_fd, &allset);
        maxfd = serv_fd;

        for(i = 0; i < MAXSOCKET; i++)
        {
            if (client[i] != -1)
            {
                FD_SET(client[i], &allset);
                if (client[i] > maxfd)
                {
                    maxfd = client[i];
                }
            }
        }
        rc = select(maxfd + 1, &allset, NULL, NULL, NULL);

        if (rc < 0)
        {
            printf("Select failed! error message:%s\n",strerror(errno));
            break;
        }
        if (FD_ISSET(serv_fd, &allset))
        {
            int clnt_fd = server_accept(serv_fd);
            if (clnt_fd < 0)
            {
                break;
            }
            setnonblock(clnt_fd);
            for(i = 0; i < MAXSOCKET; i++)
            {
                if (client[i] == -1)
                {
                    client[i] = clnt_fd;
                    break;
                }
            }
            if (i == MAXSOCKET)
            {
                close_socket(clnt_fd);
            }
        }
        for (i = 0; i < MAXSOCKET; i++)
        {
            if (client[i] == -1)
            {
                continue;
            }
            if (FD_ISSET(client[i], &allset))
            {
                if (socket_recv(client[i]) < 0)
                {
                    close_socket(client[i]);
                    client[i] = -1;
                }
            }
            rc--;
        }
        if (rc < 0)
        {
            break;
        }
    }
    close_socket(serv_fd);
    return 0;
}
