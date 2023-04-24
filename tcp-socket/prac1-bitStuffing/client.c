#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define max 100
#define path "127.0.0.10"

void main(){
	
	int cs = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(path);
	saddr.sin_port=htons(1234);

	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	
	while(1){
		
		char buff[max];
		
		printf("Enter the message: \n");
		fgets(buff, max, stdin);
		buff[strlen(buff)-1] = '\0';

		write(cs, buff, max);

		sleep(1);
		
		if(strncmp(buff, "exit", 4)==0) break;

		read(cs, buff, max);

		printf("Bit stuffed message: %s\n", buff);

	}
	close(cs);


}
