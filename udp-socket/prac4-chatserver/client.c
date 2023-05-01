#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

#define max 100
#define sip "127.0.0.1"
#define sp 2762
#define cip "127.0.0.10"
#define cp 2262

pthread_t r, w;

void* sender(void* args){
	
	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(sip);
	saddr.sin_port = htons(sp);

	caddr.sin_family = AF_INET;
	caddr.sin_addr.s_addr = inet_addr(cip);
	caddr.sin_port = htons(cp);

	bind(ss, (struct sockaddr *)&caddr, sizeof(caddr));

	while(1){
		char buff[max];

		printf("Enter the message to be sent: \n");
		fgets(buff, max, stdin);
		buff[strlen(buff)-1] = '\0';

		int length = sizeof(saddr);
		sendto(ss, buff, max, 0, (struct sockaddr *)&saddr, length);
		
		if(strncmp(buff, "exit", 4)==0) break;

	}

	close(ss);

}
void* receiver(void* args){
	
	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(sip);
	saddr.sin_port = htons(sp);

	caddr.sin_family = AF_INET;
	caddr.sin_addr.s_addr = inet_addr(cip);
	caddr.sin_port = htons(cp);

	bind(ss, (struct sockaddr *)&caddr, sizeof(caddr));

	while(1){
		char buff[max];


		int length = sizeof(saddr);
		recvfrom(ss, buff, max, 0, (struct sockaddr *)&saddr, &length);

		printf("Message from server: %s\n", buff);
		
		if(strncmp(buff, "exit", 4)==0) break;

	}

	close(ss);

}

void main(){
	
	pthread_create(&r, NULL, (void *)receiver, NULL);
	pthread_create(&w, NULL, (void *)sender, NULL);

	pthread_join(r, NULL);
	pthread_join(w, NULL);
} 
