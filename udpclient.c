#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1

struct member
{ 	int itemnumber,quantity;
	char title[100];
};

int main(int argc,char *argv[])
{
	int i,v,x,sock,serv_len,client_item,client_quantity;
	struct sockaddr_in remote_server;
	struct member receiver;


	if((sock = socket(PF_INET,SOCK_DGRAM,0))==ERROR)
		{	
			perror("Socket");
			exit(-1);
		}

	remote_server.sin_family=AF_INET;
	remote_server.sin_addr.s_addr=inet_addr(argv[1]);
	remote_server.sin_port=htons(atoi(argv[2]));

	serv_len=sizeof(remote_server);
	
		if(strcmp(argv[3],"list")==0)
		{ 
			x=1;
			printf("Item Number	  Title		Quantity\n");		
			sendto(sock,&x,sizeof x,0,(struct sockaddr *)&remote_server,serv_len);
	
				for(i=0;i<3;i++)
				{
					recvfrom(sock,&receiver,sizeof receiver,0,(struct sockaddr *)&remote_server,&serv_len);
		
					printf("  %d		%s	   %d\n",receiver.itemnumber,receiver.title,receiver.quantity);
				}
		}
	
		else if(strcmp(argv[3],"order")==0)	
		{ 
			x=2;
			client_item=atoi(argv[4]);	
			client_quantity=atoi(argv[5]);
	
			sendto(sock,&x,sizeof x,0,(struct sockaddr *)&remote_server,serv_len);
			sendto(sock,&client_item,sizeof client_item,0,(struct sockaddr *)&remote_server,serv_len);
			sendto(sock,&client_quantity,sizeof client_quantity,0,(struct sockaddr *)&remote_server,serv_len);
	
			recvfrom(sock,&v,sizeof v,0,(struct sockaddr *)&remote_server,&serv_len);
	
				if(v==1)
				{
					printf("OK\n");
				}
				
				else if(v==0)
				{
					printf("Server don't have sufficient quantity of required DVD\n");
				}
		}

		else if(argc<2)
		{
			printf("\nError\n");

		}
		else
		{
			printf("Wrong input");
		}	

close(sock);
return 0;
}
