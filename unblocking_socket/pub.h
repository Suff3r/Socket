/*************************************************************************
	> File Name: pub.h
	> Author: 
	> Mail: 
	> Created Time: Wed 15 Aug 2018 04:59:54 AM PDT
 ************************************************************************/

#ifndef _PUB_H
#define _PUB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>//htons()函数头文件
#include <netinet/in.h>//inet_addr()头文件
#include <fcntl.h>

//服务器创建socket
int server_socket(int port);

//设置非阻塞
int setnonblock(int st);

//接收客户端socket
int server_accept(int st);

//关闭socket
int close_socket(int st);

//接收消息
int socket_recv(int st);

//连接服务器
int connect_server(char *ipaddr,int port);

//发送消息
int socket_send(int st);

//将sockaddr_in转化成IP地址
void sockaddr_toa(const struct sockaddr_in * addr, char * ipaddr);

#endif
