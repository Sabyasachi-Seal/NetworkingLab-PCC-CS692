#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define max 100
#define ip "127.0.0.1"
#define port 1234
#define N 5

void main(){
	int cs = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;

	saddr.sin_addr.s_addr = inet_addr(ip);
	saddr.sin_port = htons(port);

	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));
	
	while(1){
		
		char buff[max];

		printf("Enter the message: \n");
		fgets(buff, max, stdin);
		buff[strlen(buff)-1] = '\0';

		write(cs, buff, max);

		sleep(1);

		read(cs, buff, max);

		printf("Reply from server: %s\n", buff);

		if(strncmp(buff, "exit", 4)==0) break;


	}
	close(cs);
	
}


