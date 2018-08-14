/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Sun 12 Aug 2018 05:06:42 AM PDT
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 10

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Send msg to everyone connecting the server*/
void send_msg(char *msg, int len){
	int i,numbytes;
	pthread_mutex_lock(&mutex);
	for (i = 0; i < clnt_cnt; i++){
		numbytes = send(clnt_socks[i], msg, len, 0);
		if (numbytes < 0){
			printf("Send failed\n");
		}
    }
	pthread_mutex_unlock(&mutex);	
}

void *handle_clnt(void* arg){
	int i;
	int msg_len = 0;
	char msg[BUF_SIZE] = {0};
	int clnt_sock = *(int *)arg;
	
	//while(0 != (msg_len = recv(clnt_sock, msg, sizeof(msg), 0)))
	//	send_msg(msg, msg_len);
	while(1){
		msg_len = recv(clnt_sock, msg, sizeof(msg), 0);
		if (0 > msg_len){
			printf("Receive failed\n");
			return (void *)-1;
		}else if (0 == msg_len){
			printf("Client disconnect\n");
			break;
		}
		send_msg(msg, msg_len);
		memset(&msg, 0, BUF_SIZE);
	}
    //delete current sock_fd from the shuzu
	pthread_mutex_lock(&mutex);
	for (i = 0; i < clnt_cnt; i++){
		if (clnt_sock == clnt_socks[i]){
			while (i++ < clnt_cnt -1)
				clnt_socks[i] = clnt_socks[i + 1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutex);
	
	close(clnt_sock);
}

int main(int argc, const char* argv[]){
	pthread_t tid;
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr,clnt_addr;
	socklen_t socklen = sizeof(struct sockaddr);
	if (argc != 2){
		printf("You should input like: %s <port>\n", argv[0]);
		return -1;
	}

	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (-1 == serv_sock){
		printf("Creat socket failed\n");
		return -1;
	}
	printf("Creat socket success!\n");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (-1 == bind(serv_sock, (struct sockaddr*)&serv_addr, socklen)){
		printf("Bind error\n");
		return -1;
	}
	printf("Bind succes\n");
	if (-1 == listen(serv_sock, 10)){
		printf("listen error\n");
		return -1;
	}
	printf("Listening...\n");
	while(1){
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &socklen);
		if (-1 == clnt_sock){
			printf("Accept failed\n");
			return -1;
		}
		printf("Accept by %s\n",inet_ntoa(clnt_addr.sin_addr));

		pthread_mutex_lock(&mutex);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutex);

		pthread_create(&tid, NULL, handle_clnt, (void *)&clnt_sock);
		pthread_detach(tid);
	}

	close(serv_sock);
	return 0;
}


