#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define max 100
#define multicast_group "192.168.1.11"
#define multicast_port 4321

void main(int args){
	struct ip_mreq mreq;
	int reuse = 1;
	int ss = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	struct sockaddr_in saddr;
	char buff[max];
	int len = sizeof(saddr);
	bzero ((char *) &saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(multicast_port);
	if(args>1){
		saddr.sin_addr.s_addr = inet_addr(multicast_group);
	       	while(1){
			printf("Enter the message: \n");
			fgets(buff, max, stdin);
			buff[strlen(buff)-1] = '\0';
			sendto(ss, buff, max, 0, (struct sockaddr *)&saddr, len);
			sleep(1);	
		}	
	}
	else{
		bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));
		mreq.imr_multiaddr.s_addr = inet_addr(multicast_group);
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		setsockopt(ss, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
		while(1){
			if(recvfrom(ss, buff, max, 0, (struct sockaddr *)&saddr, &len) == 0){
				break;
			}
			printf("Message received: %s\n", buff);
		}
	}
}
