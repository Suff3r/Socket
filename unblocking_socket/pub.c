/*************************************************************************
	> File Name: pub.c
	> Author: 
	> Mail: 
	> Created Time: Wed 15 Aug 2018 05:02:39 AM PDT
 ************************************************************************/

#include<pub.h>

#define MAXBUF 1024

int socket_create()
{
    int st = socket(AF_INET, SOCK_STREAM, 0);
    if (st == -1)
    {
        printf("Creat socket failed! error message:%s\n",strerror(errno));
        return -1;
    }
    return st;
}

int socket_reuseaddr(int st)
{
    int on = 1;
    if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizof(on)) == -1)
    {
        printf("Creat socket failed! error message:%s\n",strerror(errno));
        close_socket(st);
        return -1;
    }
    return 0;
}

//bind and listen
int socket_bind(int st, int port)
{
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(st, (struct sockadrr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        printf("Bind failed! error message:%s\n", strerror(errno));
        close_socket(st);
        return -1;
    }
    if (listen(st, 10) == -1)
    {
        printf("Bind failed! error message:%s\n", strerror(errno));
        close_socket(st);
        return -1; 
    }
    return 0;
}

//server create socket
int server_socket(int port)
{
    int st = socket_create();
    if (st == -1)
    {
        return -1;
    }
    //reuseaddr
    if (socket_reuseaddr(st) == -1)
    {
        return -1;
    }
    //bind and listen
    if (socket_bind(st,port) == -1)
    {
        return -1;
    }
    return st;
}
//connect server
int connect_server(char *ipaddr, int port)
{
    if (port < 0 || ipaddr == NULL)
    {
        printf("params failed\n");
        return -1;
    }
    int st = socket_create();
    if (st < 0)
    {
        return -1;
    }

    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    clnt_addr.sin_family = AF_INET;
    clnt_addr.sin_port = htons(port);
    clnt_addr.sin_addr_s_addr = inet_addr(ipaddr);
    if (connect(st, (struct sockaddr_in*)&clnt_addr, sizeof(clnt_addr)) == -1)
    {
        printf("connect failed! error message:%s\n",strerror(errno));
        return -1;
    }
    return st;
}

//set unblocking socket
int setnonblock(int st)
{
    int opts = fcntl(st, F_GETFL);
    if (opts < 0)
    {
        printf("Fcntl failed! error message:%s\n", strerror(errno));
        return -1;
    }
    opts |= O_NONBLCOK;
    if (fcntl(st, F_SETFL, opts) < 0)
    {
        printf("Fcntl failed! error message:%s\n", strerror(errno));
        return -1;
    }
    return opts;
}

void sockaddr_toa(const struct sockaddr_in* addr, char *ipaddr)
{
    unsigned char *p = (unsigned char *)&(addr.sin_addr.s_addr);
    sprintf(ipaddr, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
}

int server_accept(int st)
{
    int clnt_st;
    char *ipaddr;
    struct sockaddr_in clnt_addr;
    socklen_t len = sizeof(clnt_addr);
    memset(&clnt_addr, 0, len);
    
    clnt_st = accept(st, (struct sockaddr*)&clnt_addr, &len);
    if (clnt_st == -1)
    {
        printf("Accept client failed! error message:%s\n", strerror(errno));
        return -1;
    }
    sockaddr_toa(clnt_addr, ipaddr);
    printf("Accept by %s\n", ipaddr);

    return clnt_st;
}

int close_socket(int st)
{
    if (st < 0)
    {
        printf("close_socket params wrong\n");
        return -1;
    }
    close(st);
    return 0;
}

int socket_recv(int st)
{
    int ret = 0;
    char buf[MAXBUF] = {0};

    if (st < 0)
    {
        printf("close_socket params wrong\n");
        return -1;
    }
    ret = recv(st, buf, sizeof(buf), 0);
    if (ret == 0)
    {
        printf("client disconnet\n");
        return -1;
    }
    else if(ret < 0)
    {
        printf("Receive failed! erro message:%s\n", strerror(errno));
        return -1;
    }
    printf("%s",buf);
    
    return 0;
}

int socket_send(int st)
{
    char buf[MAXBUF] = {0};
    while(1)
    {
        printf("Input something\n");
        read(STDIN_FILENO, buf, sizeof(buf));
        if (send(st, buf, strlen(buf), 0) < 0)
        {
            printf("Send failed\n");
            return -1;
        }
        memset(buf, 0, sizeof(buf));
    }
    return 0;
}
