#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


#define max 100
#define ip "192.168.1.11"
#define port 6767

void main(int args){

	struct ip_mreq mreq;

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	int reuse = 1;
	setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	if(args>1){
	
		saddr.sin_addr.s_addr = inet_addr(ip);

		setsockopt(ss, IPPROTO_IP, SO_BROADCAST, &reuse, sizeof(reuse));

		while(1){
			
			char buff[max];
			
			printf("Enter message: \n");
			fgets(buff, max, stdin);
			buff[strlen(buff)-1] = '\0';

			sendto(ss, buff, max, 0, (struct sockaddr *)&saddr, sizeof(saddr));

			sleep(1);
		
		}

	}
	else{
	
		bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));


		while(1){
		
			char buff[max];
			int len = sizeof (saddr);
			recvfrom(ss, buff, max, 0, (struct sockaddr *)&saddr, &len);

			printf("Received: %s\n", buff);
		
		}

	}
}
