#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

#define max 100
#define path "mysock"

void main(){
	// remove previous socket

	// make server side socket
	int cs = socket(AF_UNIX, SOCK_STREAM, 0);

	// binding process
	struct sockaddr_un saddr, caddr;
	saddr.sun_family = AF_UNIX;
	strcpy(saddr.sun_path, path);

	// now bind
	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
		// server reads request first
		
		char buff[max];

		printf("Enter some data: \n");
	
		read(0, buff, max);

		buff[strlen(buff)-1] = '\0';

		write(cs, buff, max);	

		read(cs, buff, max);

		printf("Message revieved from sever: %s\n", buff);


	}
}

