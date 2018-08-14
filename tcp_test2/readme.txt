tcp多线程并发服务器

tcp多线程并发服务器框架
int main()
{
    创建套接字sockfd
    bind套接字sockfd
    listen套接字sockfd
    
    while（1）
    {
        int new_fd = accept();
        pthread_t tid;
        pthread_creat();
        pthread_detach(tid);
    }
    close(sockfd);
    return 0;
}
void *client_fun(void *arg)
{
    int new_fd = (int)arg;
    fun();//服务于客户端的具体程序
    close（new_fd）;
}

为什么网络服务器与客户端框架不用pthread_join();而是用pthread_detach（）？
答：pthread_join函数等待其他线程终止，然后被显示地回收其资源
而调用pthread_detach函数可以使其分离，也可以进行回收资源，是立即返回的。
当做TCP网络端的多线程的时候，主线程调用了pthread_join时候，子线程如果没有结束，那么主线程会被阻塞，所以可以调用pthread_detach（子线程id），来将子线程的状态设定为分离的，线程运行结束后会自动释放所有资源。
