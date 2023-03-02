#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include <sys/un.h>

void main(){
	//creating server socket ss
	int cs= socket(AF_UNIX, SOCK_STREAM, 0);
	
	//defining structure to store address of server 
	struct sockaddr_un saddr;
	saddr.sun_family= AF_UNIX;
	strcpy(saddr.sun_path, "mySock");
	
	//connecting
	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));
	

	//sending connection requests and receiving data
	
	while(1){
		char data[100];
		//writing
		printf("Enter message to be sent: ");
		fgets(data, sizeof(data), stdin);
		data[strlen(data)-1]= '\0';
		if(strcmp(data, "EXIT")==0 || strcmp(data, "exit")==0)	//exit condition
			break;
		write(cs, (void *)data, strlen(data)+1);
		//printf("Message sent to the server: %s\n", data);
		
		sleep(1);
		
		//reading
		read(cs, (void *)data, sizeof(data));
		//printf("Read from client\n");
		printf("Message read: %s\n\n", data);	
				
	}
	
	//closing the temp socket
	close(cs);
}
