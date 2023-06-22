#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
void main(){

	int cs = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(1234);

	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
	
		char buff[100];

		int len = sizeof(saddr);
		fgets(buff, 100, stdin);
		buff[strlen(buff)-1] = '\0';

		sendto(cs, buff, 100, 0, (struct sockaddr *)&saddr, len);
		
		recvfrom(cs, buff, 100, 0, (struct sockaddr *)&saddr, &len);

		printf("Server says: %s\n",buff);

	
	}

}
