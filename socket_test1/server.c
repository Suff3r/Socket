/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Aug 2018 06:22:10 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<errono.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc, const char* argv[])
{
    int fd, new_fd, struct_len, numbytes;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    //create socket
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    //fill the struct sockaddr_in and bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8600);
    server_addr.sin_addr.s_addr = 
    //listen
    //accept
    //send and recv
}
