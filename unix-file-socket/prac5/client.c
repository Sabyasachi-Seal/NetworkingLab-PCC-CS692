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

	int cs = socket(AF_UNIX, SOCK_STREAM, 0);

	struct sockaddr_un saddr, caddr;
	saddr.sun_family = AF_UNIX;
	strcpy(saddr.sun_path, path);

	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	

	while(1){
		
		char buff[max];

		printf("Enter some message: \n");

//		int n = read(0, buff, max);

		fgets(buff, max, stdin);

		int n = strlen(buff);

		buff[n-1] = '\0';

		write(cs, (void *)buff, n+1);

		if(strncmp(buff, "exit", 4)==0) break;

		read(cs, (void *)buff, sizeof(buff));

		printf("Sever Says: %s\n", buff);
	}
	close (cs);
	
	

}
