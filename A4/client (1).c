#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include <sys/un.h>

typedef struct crcData{
	char msg[100];
	char divisor[20];
}crc;

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
		//creating variable to store msg and divisor
		crc data;
		
		//writing
		
		//getting msg as input
		printf("Enter message to be sent: ");
		fgets(data.msg, sizeof(data.msg), stdin);
		data.msg[strlen(data.msg)-1]= '\0';
		//printf("Message read: %s\n\n", data.msg);	
		
		
		//checking exit condition
		if(strcmp(data.msg, "EXIT")==0 || strcmp(data.msg, "exit")==0)	//exit condition
			break;
			
		//getting divisor(polynomial) as input
		int n;
		printf("Enter max power of x: ");
		scanf("%d", &n);
		getchar();	//to fix scanf leaving a '\n'
		
		printf("Enter 1/0 (without space) for presence/absence of x^n starting with x^%d, x^%d, ... x^0: ", n, n-1);
		fgets(data.divisor, sizeof(data.divisor), stdin);
		data.divisor[strlen(data.divisor)-1]= '\0';
		//printf("Message read: %s\n\n", data.divisor);
		
		//adding additional bits to msg
		char ch= '0';
		for(int i=0; i<n; i++)
			strncat(data.msg, &ch, 1);
			
		//printf("msg sent: %s\n", data.msg);
		
		//sending data to server
		write(cs, (void *)&data, sizeof(data));
		//printf("Message sent to the server: %s\n", data);
		
		sleep(1);
		
		//reading
		read(cs, (void *)&data, sizeof(data));
		//printf("Read from client\n");
		printf("Message received: %s\n\n", data.msg);	
				
	}
	
	//closing the temp socket
	close(cs);
}

//101110010000
