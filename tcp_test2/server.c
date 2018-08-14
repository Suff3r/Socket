/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: Sat 11 Aug 2018 06:25:15 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>

typedef struct _recvmodel
{
    int fd;
    struct sockaddr_in * addr;
}RecvModel;

void *send_thread(void *arg)
{
	printf("fd = %d\n",*(int *)arg);
    int fd = *(int *)arg;
    char buf[64] = {0};
    while(1)
    {
        printf("Input something\n");
        read(STDIN_FILENO, buf, sizeof(buf));
        if (send(fd, buf, strlen(buf), 0) == -1)
        {
            printf("send failed!\n");
            return NULL;
        }
        memset(buf, 0, sizeof(buf));
    }
}

void *recv_thread(void *arg)
{
    RecvModel *model = (RecvModel *)arg;
    int flag = 0;
    char buf[64] = {0};
    while(1)
    {
        flag = recv(model->fd, buf, sizeof(buf), 0);
        if (0 == flag){
            printf("Client close the socket\n");
            return NULL;
        }else if (-1 == flag){
            printf("receive failed\n");
            return NULL;
        }
        printf("from %s:%s", inet_ntoa(model->addr->sin_addr),buf);
        memset(buf, 0, sizeof(buf));
    }
}
int main(int arg, char *argv[])
{
    int sock_fd,new_fd;
    char str[] = "Fuck WxW";
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    pthread_t thrd1,thrd2;
    socklen_t socklen = sizeof(struct sockaddr);

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        printf("Open socket failed\n");
        return -1;
    }
    printf("Creat server socket\n");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8080);
    while(-1 == bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)));
    printf("Bind succes\n");
    while(-1 == listen(sock_fd, 20));
    printf("Listening...\n");
    new_fd = accept(sock_fd, (struct sockaddr*)&clnt_addr, &socklen);
    if (-1 == new_fd)
    {
        printf("Accept failed\n");
        return -1;
    }

    RecvModel model;
    model.fd = new_fd;
    model.addr = &clnt_addr;
    printf("Accept by %s",inet_ntoa(clnt_addr.sin_addr));

    //open multithread
    if (pthread_create(&thrd1, NULL, recv_thread, (void *)&model) != 0)
    {
        printf("Creat thread failed\n");
        return -1;
    } 
    if (pthread_create(&thrd2, NULL, send_thread, (void *)&new_fd) != 0)
    {
        printf("Creat thread failed\n");
        return -1;
    }
    
    pthread_join(thrd1,NULL);
    pthread_join(thrd2,NULL);
    printf("Shutdown\n");
    //关闭套接字
    close(new_fd);
    close(sock_fd);
    return 0;
}
