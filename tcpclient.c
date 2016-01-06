#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<strings.h>
#include<arpa/inet.h>
#include<string.h>

#define ERROR -1

struct member
{ 	
	int itemnumber,quantity;
	char title[100];
};	

main(int argc,char **argv)
{
		struct sockaddr_in remote_server;
		int sock,i,x,client_item,client_quantity,v;
		struct member receiver;
	
		if((sock = socket(AF_INET,SOCK_STREAM,0))==ERROR)
		{	
			perror("Socket:");
			exit(-1);
		}

		remote_server.sin_family=AF_INET;
		remote_server.sin_port=htons(atoi(argv[2]));
		remote_server.sin_addr.s_addr=inet_addr(argv[1]);
		bzero(&remote_server.sin_zero,8);

		if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==ERROR)

		{	perror("Connect:");
			exit(-1);
		}

		if(strcmp(argv[3],"list")==0)
	
		{	printf("Item Number	  Title		Quantity\n");
			x=1;
			send(sock,&x,sizeof x,0);

			for(i=0;i<3;i++)
			{
				recv(sock,&receiver,sizeof receiver,0);
	
				printf("   %d		%s	  %d\n",receiver.itemnumber,receiver.title,receiver.quantity);
			}
		}
	
		else if(strcmp(argv[3],"order")==0)	
		{ 
			x=2;
			client_item=atoi(argv[4]);	
			client_quantity=atoi(argv[5]);
	
			send(sock,&x,sizeof x,0);
			send(sock,&client_item,sizeof client_item,0);
			send(sock,&client_quantity,sizeof client_quantity,0);
	
			recv(sock,&v,sizeof v,0);
		
			if(v==1)
			{
				printf("OK\n");
			}
	
			else if(v==0)
			{	
				printf("Server don't have sufficient quantity of required DVD\n");
			}	
		}	

		else
		{	
			printf("Error\n");
		}


		close(sock);
}
