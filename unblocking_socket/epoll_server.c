/*************************************************************************
	> File Name: epoll_server.c
	> Author: 
	> Mail: 
	> Created Time: Thu 16 Aug 2018 05:10:23 AM PDT
 ************************************************************************/

#include"pub.h"
#include<sys/epoll.h>

#define MAXSOCKET 20

int main(int argc, const char* argv[])
{
    int serv_st;
    //生成用于处理accept的epoll专用文件描述符 
    int epfd;
    /*
     * 声明epoll_event结构体变量ev，变量ev用于注册事件，
     * 数组events用于回传需要处理的事件
     */
    struct epoll_event ev, events[100];
    if (argc < 2)
    {
        printf("Input like this:%s <port>",argv[0]);
        return -1;
    }
    serv_st = server_socket(atoi(argv[1]));
    if (serv_st == -1)
    {
        return -1;
    }

    epfd = epoll_create(MAXSOCKET);
    setnonblock(serv_st);

    ev.data.fd = serv_st;
    //设置这个文件描述符需要epoll监控的事件
    /*
     * EPOLLIN代表文件描述符读事件
     * accept,recv都是读事件
     */
    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    /*
     * 注册epoll事件
     * 函数epoll_ctl中&ev参数表示需要epoll监视的serv_st中的一些事件
     */
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_st, &ev);

    while(1)
    {
        /*
         * 等待epoll池中的socket发生事件，这里一般设置为阻塞的
         * events这个参数的类型是epoll_event类型的数组
         * 如果epoll池中的一个或者多个socket发生事件，
         * epoll_wait就会返回，参数events中存放了发生事件的socket和这个socket所发生的事件
         * 这里强调一点，epoll池存放的是一个个socket，不是一个个socket事件
         * 一个socket可能有多个事件，epoll_wait返回的是有消息的socket的数目
         * 如果epoll_wait返回事件数组后，下面的程序代码却没有处理当前socket发生的事件
         * 那么epoll_wait将不会再次阻塞，而是直接返回，参数events里面的就是刚才那个socket没有被处理的事件
         */
        int nfds = epoll_wait(epfd, events, MAXSOCKET, -1);
        if (nfds == -1)
        {
            printf("epoll_wait failed!error message:%s\n", strerror(errno));
            break;
        }
        int i = 0;
        for(; i < nfds; i++)
        {
            if (events[i].data.fd < 0)
            {
                continue;
            }
            if (events[i].data.fd == serv_st)
            {
                int clnt_st = server_accept(serv_st);
                if (clnt_st < 0)
                {
                    continue;
                }
                setnonblock(clnt_st);
                //add clnt_st to epoll
                struct epoll_event clnt_ev;
                clnt_ev.data.fd = clnt_st;
                clnt_ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_st, &clnt_ev);
                /*
                 * 注释：当epoll池中listen_st这个服务器socket有消息的时候
                 * 只可能是来自客户端的connect消息
                 * recv,send使用的都是客户端的socket，不会向listen_st发送消息的
                 */
                continue;
            }
            //client st has events
            if (events[i].events & EPOLLIN)
            {
                if (socket_recv(events[i].data.fd) < 0)
                {
                    close_socket(events[i].data.fd);
                    events[i].data.fd = -1;
                    continue;
                }
            }
            if (events[i].events & EPOLLERR)
            {
                printf("EPOLLERR\n");
                close_socket(events[i].data.fd);
                events[i].data.fd = -1;
                continue;
            }
            if (events[i].events & EPOLLHUP)
            {
                printf("EPOLLHUP\n");
                close_socket(events[i].data.fd);
                events[i].data.fd = -1;
                continue;
            }
        }
    }
    close(epfd);
    close_socket(serv_st);
    return 0;
}
