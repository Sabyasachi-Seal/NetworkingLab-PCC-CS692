#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define ip "192.168.1.11"
#define port 5432
#define max 100

void main(int args){

	struct ip_mreq mreq;

	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	int reuse = 1;
	setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	
	char buff[max];

	if(args > 1){
		//send
		saddr.sin_addr.s_addr = inet_addr(ip);
		setsockopt(ss, IPPROTO_IP, SO_BROADCAST, &reuse, sizeof(reuse));
		while(1){
		
			printf("Enter something: \n");	
			fgets(buff, max, stdin);
			buff[strlen(buff)-1] = '\0';

			int len = sizeof(saddr);
			sendto(ss, buff, max, 0, (struct sockaddr *)&saddr, len);
		}
	}
	else{
		bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));


		while(1){
			

			int len = sizeof(saddr);
			recvfrom(ss, buff, max, 0, (struct sockaddr *)&saddr, &len);

			printf("Received: %s\n", buff);
		}
	
	}






}
