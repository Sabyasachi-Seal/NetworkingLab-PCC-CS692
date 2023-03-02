#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
//#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define add "127.0.0.1"

void main(){
	//creating server socket ss
	int cs= socket(AF_INET, SOCK_STREAM, 0);
	
	//defining structure to store address of server 
	struct sockaddr_in saddr;
	saddr.sin_family= AF_INET;
//	strcpy(saddr.sun_path, "mySock");
	saddr.sin_addr.s_addr = inet_addr(add);
	saddr.sin_port = 1234;
	
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
