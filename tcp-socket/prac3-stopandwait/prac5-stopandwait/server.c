#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void main(){

	int ss = socket(AF_INET, SOCK_STREAM, 0);
	int seed = 10001000, ack=0, r, delay;

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(4321);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	int l = sizeof(saddr);
	int ts = accept(ss, (struct sockaddr *)&saddr, &l);

	srand(seed);
	while(1){
		
		read(ts, &r, sizeof(r));
		if(r==ack){
			printf("Received \n");
			ack = (ack+1)%2;
		}
		else{
			printf("Duplicate recived\n");
		}
		
		delay = (rand()%7)+1;
		sleep(delay);
		write(ts, &ack, sizeof(ack));
		

	}

}
