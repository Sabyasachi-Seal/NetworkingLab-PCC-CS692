#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define path "mysock"

void main(){
	int cs = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(1234);
	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));
	while(1){
		char buff[100];
		fgets(buff, 100, stdin);
		buff[strlen(buff)-1] = '\0';
		write(cs, buff, 100);
		read(cs, buff, 100);
		printf("Client says: %s\n",buff);
	}
}
