/*************************************************************************
    > File Name: client.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 12 Aug 2018 10:17:00 PM PDT
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>

#define BUF_SIZE  100
#define NAME_SIZE 20

char name[NAME_SIZE] = "[default]";
char msg[BUF_SIZE] = {0};

void * send_msg(void *arg){
	int numbytes;
	int sock = *((int *)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	while(1){
        read(STDIN_FILENO, msg, sizeof(msg)-1);
		sprintf(name_msg, "%s %s", name, msg);
		numbytes = send(sock, name_msg, strlen(name_msg)-1, 0);
		if (0 > numbytes){
			printf("Send failed\n");
			return (void *)-1;
		}else if (0 == numbytes){
			printf("Server disconnect\n");
			return (void *)-1;
		}
		memset(msg, 0, BUF_SIZE);
	}
}

void * recv_msg(void *arg){
	int numbytes;
	int sock = *((int *)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	while(1){
		numbytes = recv(sock, name_msg, sizeof(name_msg) - 1, 0);
		if (0 > numbytes){
			printf("Receive failed\n");
			return (void *)-1;
		}
		name_msg[numbytes] = '\0';
		printf("%s\n",name_msg);
	}
}

int main(int argc, const char* argv[]){
	int clnt_sock;
	pthread_t tid1,tid2;
	struct sockaddr_in serv_addr;
	socklen_t len = sizeof(struct sockaddr_in);

	if (argc != 4){
		printf("Input like this: %s <port> <IP> name",argv[0]);
		return -1;
	}

	sprintf(name, "[%s]", argv[3]);
	clnt_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == clnt_sock){
		printf("Create socket failed\n");
		exit(-1);
	}

	memset(&serv_addr, 0, len);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr = inet_addr(argv[2]);
	if (-1 == connect(clnt_sock, (struct sockaddr*)&serv_addr, len)){
		printf("Connect error");
		exit(-1);
	}

	if (0 != pthread_create(&tid1, NULL, send_msg, (void *)&clnt_sock)){
		printf("Thread1 create failed\n");
		exit(-1);
	}
	if (0 != pthread_create(&tid2, NULL, recv_msg, (void *)&clnt_sock)){
		printf("Thread2 create failed\n");
		exit(-1);
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	close(clnt_sock);
	return 0;
}
