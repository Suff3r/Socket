/*************************************************************************
	> File Name: udprecv.c
	> Author: 
	> Mail: 
	> Created Time: Mon 13 Aug 2018 07:06:16 AM PDT
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
    if(arg<2)
    {
        printf("please print one param !\n");
        return -1;
    }
    int st=socket(AF_INET,SOCK_DGRAM,0);
    if(st==-1)
    {
        printf("open socket failed ! error message : %s\n",strerror(errno));
        return -1;
    }
    int port=atoi(args[1]);
    //defien ip address strcut
    struct sockaddr_in addr;
    //define addr type
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    //bind port
    if(bind(st,(struct sockaddr *)&addr,sizeof(addr))==-1)
    {
        printf("bind IP failed ! error message : %s\n",strerror(errno));
        goto END;
    }
    char buf[1024]={0};
    struct sockaddr_in client_addr;
    socklen_t addrlen=sizeof(client_addr);
    while(1)
    {
        memset(&client_addr,0,sizeof(client_addr));
        if(recvfrom(st,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&addrlen)==-1)
        {
            printf("recvfrom failed ! error message : %s\n",strerror(errno));
            goto END;
        }else
        {
            printf("from %s:%s",inet_ntoa(client_addr.sin_addr),buf);
        }
        memset(buf,0,sizeof(buf));
    }
    END:close(st);
    return 0;
}
