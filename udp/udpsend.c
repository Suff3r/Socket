/*************************************************************************
	> File Name: udpsend.c
	> Author: 
	> Mail: 
	> Created Time: Mon 13 Aug 2018 07:04:37 AM PDT
 ************************************************************************/

//udp接收消息
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int arg, char * args[])
{
    if (arg < 2)
    {
        printf("please print two params!\n");
        return -1;
    }
    int port = atoi(args[2]);
    //create socket
    int st = socket(AF_INET, SOCK_DGRAM, 0);
    if (st == -1)
    {
        printf("create socket failed ! error message :%s\n", strerror(errno));
        return -1;
    }
    //UDP广播必须设置socket属性
    /*
    int on = 1;
    if (setsockopt(st, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) == -1)
    {
        printf("setsockopt failed ! error message :%s\n", strerror(errno));
        return -1;
    }
    */
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    //设置结构sockaddr_in类型是TCP/IP
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(args[1]);
    char buf[1024] = { 0 };
    while (1)
    {
        //read
        read(STDIN_FILENO, buf, sizeof(buf));
        if (sendto(st, buf, strlen(buf), 0, (struct sockaddr *) &addr,
                sizeof(addr)) == -1)
        {
            printf("sendto failed ! error message :%s\n", strerror(errno));
            break;
        }
        memset(buf,0,sizeof(buf));
    }
    close(st);
    return 0;
}
