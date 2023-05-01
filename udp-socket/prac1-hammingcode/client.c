#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define max 100
#define ip "127.0.0.1"
#define port 1234

void main(){
	
	int cs = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ip);
	saddr.sin_port = htons(port);

	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
	
		char buff[max];
		
		printf("Enter the message: \n");
		fgets(buff, max, stdin);
		buff[strlen(buff)-1] = '\0';

		int len = sizeof(saddr);
		sendto(cs, buff, max, 0, (struct sockaddr *)&saddr, len);

		recvfrom(cs, buff, max, 0, (struct sockaddr *)&saddr, &len);

		printf("Message from server: %s\n", buff);

		
	}
	close(cs);
}
