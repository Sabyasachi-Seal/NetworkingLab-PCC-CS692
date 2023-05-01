#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define max 100
#define addr "224.0.0.1"
#define port 4321

void main(int args){

	struct ip_mreq mreq;
	int reuse = 1;

	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	struct sockaddr_in saddr; 
	bzero((char *)&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	if(args>1){
		
		saddr.sin_addr.s_addr = inet_addr(addr);

		while(1){
			char buff[max];

			printf("Enter the message: \n");
			fgets(buff, max, stdin);
			buff[strlen(buff)-1] = '\0';
			
			sendto(ss, buff, max, 0, (struct sockaddr *)&saddr, sizeof(saddr));
			
			sleep(1);	
		}

	}
	else{
		
		bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

		mreq.imr_multiaddr.s_addr = inet_addr(addr);
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		setsockopt(ss, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

		while(1){
		
			char buff[max];

			int len = sizeof(saddr);
			recvfrom(ss, buff, max, 0, (struct sockaddr *)&saddr, &len);

			printf("Message recieved: %s\n", buff);

		}


	}



}
