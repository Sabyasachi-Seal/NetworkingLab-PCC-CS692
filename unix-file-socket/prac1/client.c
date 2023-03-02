#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>

#define path "mysock"

void main(){
	// first we need to make the two sockets 
	//
	int cs = socket(AF_UNIX, SOCK_STREAM, 0);

	// now we need to define the structures
	struct sockaddr_un saddr, caddr;

	unlink(saddr.sun_path);
	saddr.sun_family = AF_UNIX;
	strcpy(saddr.sun_path, path);

	// now we need to connect to the address
	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	//now the connection requests can be sent
	//
	while(1){
		
		char buff[100];
		
		printf("Enter a message: \n");

		int n = read(0, buff, sizeof(buff));

		buff[n-1] = '\0';

		if(strcmp(buff, "exit") == 0) break;

		write(cs, buff, n+1);

		sleep(1);

		read(cs, buff, sizeof(buff));

	}
	close(cs);
}
