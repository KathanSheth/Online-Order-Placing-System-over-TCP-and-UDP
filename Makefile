KK	=	gcc
TARGET	=	tcpudp	tcpclient	udpclient

all	:	$(TARGET)

tcpudp	:	tcpudp.c
	$(KK)	-o	tcpudp	tcpudp.c	-lpthread
tcpclient	:	tcpclient.c
		$(KK)	-o	tcpclient	tcpclient.c

udpclient	:	udpclient.c
		$(KK)	-o	udpclient	udpclient.c
