#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

void main(){
	struct sockaddr_in saddr,caddr;
	int r, ack=0,seed=10001,delay;

	int ss=socket(AF_INET,SOCK_STREAM,0);

	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(5555);
	inet_aton("127.0.0.1",&saddr.sin_addr);
	bind(ss,(struct sockaddr *)&saddr,sizeof(saddr));

	listen(ss,1);

	int l=sizeof(caddr);
	int ts=accept(ss,(struct sockaddr *)&caddr,&l);

	srand(seed);
	while(1){
		read(ts,(void *)&r,sizeof(r));
		if(r==ack){
			printf("\nReceived sequence number: %d\n",r);
			ack=(ack+1)%2;
		}
		else
			printf("\nDuplicate sequence number received: %d\n",r);
			delay=(rand()%7)+1;
			sleep(delay);
			write(ts,(void *)&ack,sizeof(ack));
			printf("\nACK sent for sequence number: %d\n",ack);
	}
}
