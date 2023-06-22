#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
//#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define add "127.0.0.1"

void main(){
	//creating server socket ss
	int cs= socket(AF_INET, SOCK_STREAM, 0);
	int seq, ack=0, seed=10001, delay;	

	//defining structure to store address of server 
	struct sockaddr_in saddr, caddr;
	saddr.sin_family= AF_INET;
//	strcpy(saddr.sun_path, "mySock");
	saddr.sin_addr.s_addr = inet_addr(add);
	saddr.sin_port = 1234;
	
	//connecting
	bind(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	listen(cs, 5);

	int len = sizeof(caddr);
	int ts = accept(cs, (struct sockaddr *)&caddr, &len);

	//sending connection requests and receiving data
	srand(seed);
	while(1){
		read(ts, (void*)&seq, sizeof(seq));
		if(seq == ack){
			printf("Correct ack\n");
			ack = (ack+1)%2;
		}
		else{
			printf("Duplicate received\n");
		}
		delay = (rand() % 7)+1;
		sleep(delay);
		
		write(ts, (void *)&ack, sizeof(ack));
		printf("ACK Sent %d\n", ack);
	}
	//closing the temp socket
	close(cs);
}
