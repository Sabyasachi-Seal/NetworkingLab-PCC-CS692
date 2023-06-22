#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define add "127.0.0.1"

void main(){
	
	int ss = socket(AF_INET, SOCK_STREAM, 0);
	int seq, ack=0, seed=10001, delay;

	struct sockaddr_in saddr,caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(add);
	saddr.sin_port = htons(1234);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	listen(ss, 5);

	int len = sizeof(caddr);
	int ts = accept(ss, (struct sockaddr *)&saddr, &len);

	srand (seed);
	while(1){
		
		read(ts, &seq, sizeof(seq));
		if(seq==ack){
			printf("Correct ack\n");
			ack = (ack+1)%2;
		}
		else{
			printf("Duplicate\n");
		}
		delay = (rand() % 7)+1;
		sleep(delay);
		write(ts, &ack, sizeof(ack));

	}
	close(ts);

}

