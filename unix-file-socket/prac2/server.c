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
	remove(path);

	// make server side socket
	int ss = socket(AF_UNIX, SOCK_STREAM, 0);

	// binding process
	struct sockaddr_un saddr, caddr;
	saddr.sun_family = AF_UNIX;
	strcpy(saddr.sun_path, path);

	// now bind
	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));
	
	// make listen queue and start listening
	listen(ss, 5);

	// start accepting responses, we will be using a temp socket for communication
	int len = sizeof(saddr);
	int ts = accept(ss, (struct sockaddr *)&saddr, &len);

	while(1){
		// server reads request first
		
		char buff[max];

		read(ts, buff, max);

		int n = strlen(buff);

		printf("Message revieved from client: %s\n", buff);
		
		int ones = 0, i;
		for(i=0; i<n; i++){
			if(buff[i] == '1') ones++;
		}

		if(ones%2 == 0){
			buff[n] = '0';
		}
		else{
			buff[n] = '1';
		}

		buff[n+1] = '\0';
		write(ts, buff, n+1);	

	}
	close(ts);
}

