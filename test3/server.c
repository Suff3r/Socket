/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Sun 12 Aug 2018 05:06:42 AM PDT
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100

void *handle_clnt(void* arg);
void send_msg(char *msg, int len);



