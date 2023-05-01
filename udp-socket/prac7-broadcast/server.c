#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>

#define max 100
#define ip "192.168.1.11"
#define port 4321

void main(int args){
	struct sockaddr_in saddr; 
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);
	int ss = socket(AF_INET, SOCK_DGRAM, 0);
	int reuse = 1;
	setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	int broadcast = 1;
	if(args>1){
		saddr.sin_addr.s_addr = inet_addr(ip);
		// this is for sender
		setsockopt(ss, IPPROTO_IP, SO_BROADCAST, &broadcast, sizeof(broadcast));
		while(1){
			char buff[max];
			printf("Enter some message: \n");
			fgets(buff, max, stdin);
			buff[strlen(buff)-1] = '\0';
			int len = sizeof(saddr);
			sendto(ss, buff, max, 0, (struct sockaddr *)&saddr, len);
			sleep(1);
		}
	}
	else{
		bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));
		while(1){
			char buff[max];
			int len = sizeof(saddr);
			recvfrom(ss, buff, max, 0, (struct sockaddr *)&saddr, &len);
			printf("Message received: %s\n", buff);
			sleep(1);
		}
	}
}
