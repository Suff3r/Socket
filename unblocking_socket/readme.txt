为了避免整个进程被阻塞后挂起，所以在阻塞模式下，往往需要采用多线程技术。
一个进程中可并发的线程总数是有限的，在处理大量客户端socket连接(比如上万个client socket)，通过线程并发处理socket并不方便，效率也不高。

非阻塞socket
--非阻塞调用是指调用立刻返回。
--在非阻塞模式下，accept与recv、recvfrom函数调用会立刻返回。
--在nonblocking状态下调用accept函数，如果没有客户端socket连接请求，那么accept函数返回-1，同时errno值为EAGAIN或者EWOULDBLOCK，这两个宏定义都为整数11.
--在nonblocking状态下调用recv、recvfrom函数，如果没有数据，函数返回-1，同时errno值为11（EINPROGRESS）。如果socket已经关闭，函数返回0.
--在nonblocking状态下对一个已经关闭的socket调用send函数，将引发一个SIGPIPE信号，进程必须捕捉这个信号，因为SIGPIPE系统默认的处理方式是关闭进程。

fcntl函数调用
fcntl函数可以将文件或者socket描述符设置为阻塞或者非阻塞状态
int fcntl(int fd,int cmd,.../*arg*/);
参数fd为要设置的文件描述符或者socket。
参数cmd，F_GETFL为得到目前状态，F_SETFL为设置状态。
宏定义O_NONBLOCK代表非阻塞，0代表阻塞。
成功返回值为描述符当前状态，失败返回-1，并且设置errno。

//fcntl函数调用设置非阻塞socket
int opts;

if (fcntl(st, F_SETFL, fcntl(st,F_GETFL) | O_NONBLOCK) < 0){
    printf("fcntl failed ! error message :%s\n",strerror(errno));
    return -1;   
}




