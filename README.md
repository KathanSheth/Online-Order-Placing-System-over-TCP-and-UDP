# Online-Order-Placing-System-over-TCP-and-UDP
Server file : tcpudp.c Client file : udpclient.c, tcpclient.c
- To run this program type “make” on the command prompt.
 So, all three files will be compiled simultaneously.
- To run the Server “tcpudp.c” run command: ./tcpudp 8888
 Server will wait before connect.
- To run the TCP Client “tcpclient.c” and to request list run command: ./tcpclient 127.0.0.1 8888 list
 Server will send the list to the client and also Server will keep record the quantities it has and number of TCP clients connected to server and server will print these details on server side.
- To request order from TCP client run command: ./tcpclient 127.0.0.1 8888 order 1001 10
 Server will check for the quantity and serve the order (OK if it has sufficient quantities).
- To run the UDP Client “udpclient.c” and to request list run command: ./udpclient 127.0.0.1 8888 list
 Server will send the list to the client and also Server will keep record the quantities it has and number of UDP clients connected to server and server will print these details on server side.
- To request order from UDP client run command: ./udpclient 127.0.0.1 8888 order 1002 10
 Server will check for the quantity and serve the order (OK if it has sufficient quantities).
