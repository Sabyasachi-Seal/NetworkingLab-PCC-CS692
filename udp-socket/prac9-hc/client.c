#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define ip "127.0.0.1"
#define port 1234

void main(){

	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ip);
	saddr.sin_port = htons(port);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
	
		char buff[100];

		fgets(buff, 100, stdin);
		buff[strlen(buff)-1] = '\0';

		sendto(ss, buff, 100, 0, (struct sockaddr *)&saddr, sizeof(saddr));
		
		int len = sizeof(saddr);
		recvfrom(ss, buff, 100, 0, (struct sockaddr *)&saddr, &len);

		printf("Server says: %s\n", buff);


	}
}
