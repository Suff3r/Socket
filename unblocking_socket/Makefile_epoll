.SUFFIXES:.c .o
CC=gcc
SRCS1=epoll_client.c\
	pub.c
SRCS2=epoll_server.c\
    pub.c
OBJS1=$(SRCS1:.c=.o)
OBJS2=$(SRCS2:.c=.o)
EXEC1=mclient
EXEC2=mserver

start:$(OBJS1) $(OBJS2)
	$(CC) -o $(EXEC1) $(OBJS1)
	$(CC) -o $(EXEC2) $(OBJS2)
.c.o:
	$(CC) -Wall -g -o $@ -c $<
clean:
	rm -f $(OBJS1)
	rm -f $(EXEC1)
	rm -f $(OBJS2)
	rm -f $(EXEC2)
