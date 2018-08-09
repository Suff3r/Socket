1.What's the socket
套接字，计算机之间进行通信的一种约定或方式，通过socket，一台计算机可以接收其他计算机的数据，也可以
向其他计算机发送数据。典型应用就是Web服务器和浏览器
2.数据传输方式
1) SOCK_STREAM 有连接的数据传输方式，数据传输准确但效率较慢，，损坏或丢失可以重新发送，http
2) SOCK_DGRAM  无连接的数据传输方式，只管传输数据，不作数据校验，效率高但准确性差

1. 有连接的socket
1) server: socket() -> bind() -> listen() -> accept() -> read() -> write()
2) client :                     socket() -> connect() -> write() -> read()

2.无连接的socket
1) server: socket() -> bind() -> recvfrom() -> sendto()
2) client:  socket() -> bind() -> sendto() -> recvfrom()

1、int socket(int family,int type,int protocol)l0
family:
    指定使用的协议簇:AF_INET（IPv4） AF_INET6（IPv6） AF_LOCAL（UNIX协议） AF_ROUTE（路由套接字） AF_KEY（秘钥套接字）
type:
    指定使用的套接字的类型:SOCK_STREAM（字节流套接字） SOCK_DGRAM
protocol:
    如果套接字类型不是原始套接字，那么这个参数就为0
2、int bind(int sockfd, struct sockaddr *myaddr, int addrlen)
sockfd:
    socket函数返回的套接字描述符
myaddr:
    是指向本地IP地址的结构体指针
myaddrlen:
    结构长度
struct sockaddr{
    unsigned short sa_family; //通信协议类型族AF_xx
    char sa_data[14];  //14字节协议地址，包含该socket的IP地址和端口号
};
struct sockaddr_in{
    short int sin_family; //通信协议类型族
    unsigned short int sin_port; //端口号
    struct in_addr sin_addr; //IP地址
    unsigned char si_zero[8];  //填充0以保持与sockaddr结构的长度相同
};
3、int connect(int sockfd,const struct sockaddr *serv_addr,socklen_t addrlen)
sockfd:
    socket函数返回套接字描述符
serv_addr:
    服务器IP地址结构指针
addrlen:
    结构体指针的长度
4、int listen(int sockfd, int backlog)
sockfd:
    socket函数绑定bind后套接字描述符
backlog:
    设置可连接客户端的最大连接个数，当有多个客户端向服务器请求时，收到此值的影响。默认值20
5、int accept(int sockfd,struct sockaddr *cliaddr,socklen_t *addrlen)
sockfd:
    socket函数经过listen后套接字描述符
cliaddr:
    客户端套接字接口地址结构
addrlen:
    客户端地址结构长度
accept() 返回一个新的套接字来和客户端通信，addr 保存了客户端的IP地址和端口号，而 sockfd 是服务器端的套接字，大家注意区分。后面和客户端通信时，要使用这个新生成的套接字，而不是原来服务器端的套接字。
6、int send(int sockfd, const void *msg,int len,int flags)
7、int recv(int sockfd, void *buf,int len,unsigned int flags)
sockfd:
    socket函数的套接字描述符
msg:
    发送数据的指针
buf:
    存放接收数据的缓冲区
len:
    数据的长度，把flags设置为0

请求队列 
当套接字正在处理客户端请求时，如果有新的请求进来，套接字是没法处理的，只能把它放进缓冲区，待当前请求处理完毕后，再从缓冲区中读取出来处理。如果不断有新的请求进来，它们就按照先后顺序在缓冲区中排队，直到缓冲区满。这个缓冲区，就称为请求队列（Request Queue）。

缓冲区的长度（能存放多少个客户端请求）可以通过listen() 函数的 backlog 参数指定，但究竟为多少并没有什么标准，可以根据你的需求来定，并发量小的话可以是10或者20
https://www.cnblogs.com/uestc-mm/p/7630145.html
https://blog.csdn.net/m0_37947204/article/details/80489431
https://blog.csdn.net/qq_33850438/article/details/52055390
