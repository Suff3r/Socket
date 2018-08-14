UDP的优点
--UDP不要求保持一个连接
--UDP没有因接收方没有收到数据包重传而带来开销。
--设计UDP目的是用于短应用和控制消息。
--在一个数据包接一个数据包基础上，UDP要求的网络带宽比TCP小。
UDP的缺点
--程序员必须创建代码监测数据包的正确性，必要时重传。
--程序员必须把大数据包分片。

选择使用哪一种协议？
--一些消息重要程度不高，或者有规律重复，可以使用UDP。
--如果要传输一个重要的数据，丢失一点就会破坏整个数据，那么需要选择TCP。
--telnet，ssh,http等基本都基于TCP。
--流媒体为了保证很窄的网络带宽来传送更多的数据，基本采用UDP。
多数游戏中，丢失来自某个用户的状态更新可能不会引起注意，所以采用UDP。
设计用在局域网的应用可以采用UDP，因为在局域网中丢失数据包的可能性很低。

使用UDP发送数据。
ssize_t sendto(int s,const void *buf,size_t len,int flags,const struct sockaddr *to,socklen_t tolen);

使用UDP接收数据
ssize_t recvfrom(int s,void * buf,size_t blen,int flags,struct sockaddr * from,socklen_t * fromlen);

