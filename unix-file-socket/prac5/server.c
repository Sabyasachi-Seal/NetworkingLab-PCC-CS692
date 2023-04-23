#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <sys/types.h>

#define max 100

#define path "mySock"

void main(){
	remove(path);

	int ss = socket(AF_UNIX, SOCK_STREAM, 0);

	struct sockaddr_un saddr, caddr;
	saddr.sun_family = AF_UNIX;
	strcpy(saddr.sun_path, path);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	listen(ss, 5);

	int length = sizeof(caddr);
	
	int ts = accept(ss, (struct sockaddr *)&caddr, &length);

	while(1){
		
		char buff[max];

		read(ts, (void *)buff, max);
		
		int n = strlen(buff);

		//printf("Size: %d\n", n);

		//buff[n] = '\0';

		printf("Client Says: %s\n", buff);

		// first count number of ones
		int ones = 0;
		for(int i=0; i<n; i++){
			if(buff[i] == '1') ones++;
		}
		
		if(ones&1) buff[n] = '1';
		else buff[n] = '0';

		buff[n+1] = '\0';
		
//		printf("Server edited to: \n");

//		write(1, (void *)buff, n+1);

		write(ts, (void *)buff, strlen(buff)+1);

		if(strncmp(buff, "exit", 4)==0) break;
	}
	close (ts);
	
	

}
