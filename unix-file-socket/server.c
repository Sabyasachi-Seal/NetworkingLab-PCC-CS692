#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include <sys/un.h>

//---------------------------------------------------------------EVEN PARITY SERVER---------------------------------------------------------------------

void main(){
	//creating server socket ss
	int ss= socket(AF_UNIX, SOCK_STREAM, 0);
	
	//binding server address to ss
	
	//defining structure to store address of server 
	struct sockaddr_un saddr, caddr;
	saddr.sun_family= AF_UNIX;
	strcpy(saddr.sun_path, "mySock");
	
	//binding
	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));
	
	//creating the listen queue
	listen(ss, 5);
	
	//accepting
	int length= sizeof(caddr);
	int ts= accept(ss, (struct sockaddr *)&caddr, &length);		//using *length since kernel needs to update the size in the original place
	
	
	//reading and writing of data
	
	while(1){
		//reading
		char data[100];
		read(ts, (void *)data, sizeof(data));
		printf("Read from client\n");
		printf("Message read: %s\n", data);
		
		//processing
		if(strcmp(data, "EXIT")==0 || strcmp(data, "exit")==0)	//exit condition
			break;
		//strcat(data, "hehe");					//adding hehe at end of the message
		
		int one_count= 0;
		int s= strlen(data);
		//printf("\nsize of data: %d\n", s);
		for(int i=0; i<strlen(data); i++)
			//if(strcmp(data[i], "1")==0)
			if(data[i]=='1')
				one_count++;
		if(one_count%2==0)
			data[s]= '0';
		else
			data[s]= '1';
		data[s+1]= '\0';
		//s= strlen(data);
		//printf("\nsize of data after adding parity bit: %d\n", s);
		
		//s= strlen(data);
		//printf("\nsize of data after adding null char: %d\n", s);
		//writing
		write(ts, (void *)data, strlen(data)+1);
		printf("Message sent to the client: %s\n\n", data);
		
		sleep(1);		
	}
	
	//closing the temp socket
	close(ts);
}
