#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<error.h>
#include<strings.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>
#include<sys/select.h>

#define ERROR -1
#define MAX_CLIENTS 1

struct member
{
	int itemnumber;
	int quantity;
	char title[100];
}war,pot,out;
struct sockaddr_in userver,uclient;
int y,ucli_len,usockaddr_len,client_item,client_quantity,usock,tcount=0,ucount=0;	
pthread_mutex_t mut;

int udpfunction(int usock)
{		
		(void) pthread_mutex_lock(&mut);
		ucount=ucount+1;
		printf("Total Number of UDP Client Connected%d:\n",ucount);	
		(void) pthread_mutex_unlock(&mut);
		if(y==1)
		{	(void) pthread_mutex_lock(&mut);
			printf("\nThe Client has requested for LIST\n\n");
			printf("\nItem Number       Title         Quantity\n");

			int z=	sendto(usock,&war,sizeof war,0,(struct sockaddr *)&uclient,ucli_len);
				if(z==-1)
				{
					printf("Error in Sending\n");
					exit(1);	
				}
				sendto(usock,&pot,sizeof pot,0,(struct sockaddr *)&uclient,ucli_len);
				sendto(usock,&out,sizeof out,0,(struct sockaddr *)&uclient,ucli_len);

			printf("   %d           %s        %d\n",war.itemnumber,war.title,war.quantity);
			printf("   %d           %s     %d\n",pot.itemnumber,pot.title,pot.quantity);
			printf("   %d           %s       %d\n",out.itemnumber,out.title,out.quantity);
			(void) pthread_mutex_unlock(&mut);
		}

		else if(y==2)
		{	(void) pthread_mutex_lock(&mut);
			printf("\nThe Client has requested for ORDER\n");
			int v=0;
			
			if((war.itemnumber==client_item) && (war.quantity>=client_quantity))
				{
					war.quantity=war.quantity-client_quantity;
					v=1;

					sendto(usock,&v,sizeof v,0,(struct sockaddr *)&uclient,ucli_len);
					printf("\nThe Client has requested for %d quantity of %s DVD\n",client_quantity,war.title);
				}

			else if((pot.itemnumber==client_item) && (pot.quantity>=client_quantity))
				{
					pot.quantity=pot.quantity-client_quantity;
					v=1;
					sendto(usock,&v,sizeof v,0,(struct sockaddr *)&uclient,ucli_len);  
					printf("\nThe Client has requested for %d quantity of %s DVD\n",client_quantity,pot.title);
				}
				
			else if((out.itemnumber==client_item) && (out.quantity>client_quantity))
				{
					out.quantity=out.quantity-client_quantity;
					v=1;
					sendto(usock,&v,sizeof v,0,(struct sockaddr *)&uclient,ucli_len); 
					printf("\nThe Client has requested for %d quantity of %s DVD\n",client_quantity,out.title);
				}
			else 
				{
					sendto(usock,&v,sizeof v,0,(struct sockaddr *)&uclient,ucli_len);
					printf("\nThe Client has requested for more numbers of DVDs than the Server has\n");
				}
			(void) pthread_mutex_unlock(&mut);
		}
}
int tcpfunction(int fd)
{		
		int x;
		(void) pthread_mutex_lock(&mut);
        tcount=tcount+1;
		printf("Total Number of TCP Client Connected:%d\n",tcount);	
		recv(fd,&x,sizeof x,0);
		(void) pthread_mutex_unlock(&mut);
	
	if(x==1)
	{	
		(void) pthread_mutex_lock(&mut);
		printf("\nThe Client has requested for LIST\n");
		printf("\nServer has following quantities\n");
		printf("Item Number       Title         Quantity\n");
		send(fd,&war,sizeof war,0);
		send(fd,&pot,sizeof pot,0);
		send(fd,&out,sizeof out,0);
		printf("   %d           %s        %d\n",war.itemnumber,war.title,war.quantity);
		printf("   %d           %s     %d\n",pot.itemnumber,pot.title,pot.quantity);
		printf("   %d           %s       %d\n",out.itemnumber,out.title,out.quantity);
		(void) pthread_mutex_unlock(&mut);
	}
	
	else if(x==2)
	{	
		(void) pthread_mutex_lock(&mut); 
		printf("\nThe Client has requested for ORDER\n");

		int client_item,client_quantity;
		int v=0;
	
		recv(fd,&client_item,sizeof client_item,0);
		recv(fd,&client_quantity,sizeof client_quantity,0);
		
		if((war.itemnumber==client_item) && (war.quantity>=client_quantity))
		{	
			war.quantity=war.quantity-client_quantity;
			v=1;
			send(fd,&v,sizeof v,0);
			printf("The Client has requested for %d quantity of %s DVD\n",client_quantity,war.title);
		}
		else if((pot.itemnumber==client_item) && (pot.quantity>=client_quantity))
        {
                        
        	pot.quantity=pot.quantity-client_quantity;
			v=1;
			send(fd,&v,sizeof v,0);
			printf("The Client has requested for %d quantity of %s DVD\n",client_quantity,pot.title);

        }
		else if((out.itemnumber==client_item) && (out.quantity>=client_quantity))
        {
                        
            out.quantity=out.quantity-client_quantity;
			v=1;
			send(fd,&v,sizeof v,0);
			printf("The Client has requested for %d quantity of %s DVD\n",client_quantity,out.title);

        }
		else 
		{	
			send(fd,&v,sizeof v,0);
			printf("The Client has requested for more numbers of DVDs than the Server has\n");
		}
		
		(void) pthread_mutex_unlock(&mut);

	}
}
	
int main(int argc,char **argv)
{
		struct sockaddr_in server,client;
		
		int sock,cli,nfds,tsock;
		int sockaddr_len = sizeof(struct sockaddr_in);
		fd_set rfds;
		FD_ZERO(&rfds); 
		 
				war.itemnumber = 1001;
                strcpy(war.title,"Star Wars");
                war.quantity=100;

                pot.itemnumber = 1002;
                strcpy(pot.title,"Harry Potter");
                pot.quantity=80;

                out.itemnumber = 1003;
                strcpy(out.title,"Inside Out");
                out.quantity=50;
		
	if((tsock = socket(AF_INET,SOCK_STREAM,0))==ERROR)
 	{	
		perror("Server Socket:");
		exit(-1);
	}
	printf("\nTCP Socket Created\n");
	
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr =htonl(INADDR_ANY);
	bzero(&server.sin_zero,8);

	if((bind(tsock,(struct sockaddr*)&server,sockaddr_len))==ERROR)
	{
		perror("Bind:");
		exit(-1);
	}

	if((listen(tsock,MAX_CLIENTS))==ERROR)
			{
				perror("Listen:");
				exit(-1);
		
			}
			
		usock=socket(PF_INET,SOCK_DGRAM,0);

		printf("\nUDP Socket Created\n");
		userver.sin_family=AF_INET;
		userver.sin_addr.s_addr=htonl(INADDR_ANY);
		userver.sin_port=htons(atoi(argv[1]));
		usockaddr_len=sizeof(struct sockaddr_in);
		
	if((bind(usock,(struct sockaddr*)&userver,usockaddr_len))==ERROR)
	{
		perror("Bind:");
		exit(-1);
	}

		
		printf("\nServer connected from Port number %d and IP %s\n\n",ntohs(userver.sin_port),inet_ntoa(userver.sin_addr));

		ucli_len=sizeof(uclient);
while(1)
{		nfds=getdtablesize();
        
		FD_SET(tsock, &rfds);	
		FD_SET(usock, &rfds);		
		
		pthread_t th,ti;
		pthread_attr_t ta,tb;
		(void)pthread_attr_init(&ta);
		(void)pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_JOINABLE);
		(void)pthread_attr_init(&tb);
		(void)pthread_attr_setdetachstate(&tb, PTHREAD_CREATE_JOINABLE);
		(void)pthread_mutex_init(&mut, NULL);
  		printf("\nBefore Select\n");
    
	if ((select(nfds, &rfds, (fd_set *)0, (fd_set *)0,
               (struct timeval *)0) ) < 0)
        {
		printf("select: %s\n", strerror(errno));
		exit(1);
		}	
		printf("\nSelect done\n");
	
	if (FD_ISSET(tsock, &rfds)) 
	{	
		
		if((cli = accept(tsock,(struct sockaddr*)&client,&sockaddr_len))==ERROR)

		{	perror("Accept:");
			exit(-1);
		}
	
	if (pthread_create(&th, &ta, (void * (*)(void *))tcpfunction,
		    (void *)(long)cli) < 0)
			printf("pthread_create: %s\n", strerror(errno));
			printf("\nNew client connected from port no %d and IP %s\n",ntohs(client.sin_port),inet_ntoa(client.sin_addr));
			printf("\nServer connected from port no %d and IP %s\n",ntohs(server.sin_port),inet_ntoa(server.sin_addr));

	(void)pthread_join(th,NULL);
	
	}	
	
	if (FD_ISSET(usock, &rfds))
	{		
			(void) pthread_mutex_lock(&mut);
			recvfrom(usock,&y,sizeof y,0,(struct sockaddr *)&uclient,&ucli_len);
			(void) pthread_mutex_unlock(&mut);
			
		
		if(y==2)
		{	
			(void) pthread_mutex_lock(&mut);
			recvfrom(usock,&client_item,sizeof client_item,0,(struct sockaddr *)&uclient,&ucli_len);
			recvfrom(usock,&client_quantity,sizeof client_quantity,0,(struct sockaddr *)&uclient,&ucli_len);
			(void) pthread_mutex_unlock(&mut);
		}
			int PID =pthread_create(&ti, &tb, (void * (*)(void *))udpfunction,
			(void *)(long)usock);
		    
			if(PID!=0)
		    	{
				printf("Error in creating Pthread\n");
		    	exit(1);
				}
			
			printf("\nClient connected from Port number %d and IP  %s\n",ntohs(uclient.sin_port),inet_ntoa(uclient.sin_addr));
						
		
			(void)pthread_join(ti,NULL);
	}
			
		printf("\nTotal Number of Client connected %d\n",ucount+tcount);			
}
	

close(usock);	
return 0;
}
