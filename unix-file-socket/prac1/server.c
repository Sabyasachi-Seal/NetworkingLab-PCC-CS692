#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>

#define path "mysock"

void main(){
	
	remove (path);
	
	// making socket
	int ss = socket(AF_UNIX, SOCK_STREAM, 0);

	// binding process
	// defining structure
	struct sockaddr_un saddr, caddr;
	
	saddr.sun_family = AF_UNIX;

//	unlink(saddr.sun_path);
	strcpy(saddr.sun_path, path);

	// now bind the address
	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	// making listen queue and listening
	listen(ss, 10);

	// accepting requests
	int length = sizeof(caddr); // we are taking a diff variable since we need to pass the address of this variable in the parameter of the accept fucntion
	// temporary socket
	int ts = accept(ss, (struct sockaddr *)&caddr, &length);

	while(1){
		char buff[100];
		
		// reading from the 
		int n = read(ts, buff, sizeof(buff));

		printf("Message read from client: %s\n", buff);

		if(strcmp(buff, "exit") == 0) break;

		write(ts, buff, n);

		sleep(1);
	}

	close(ts);
}
