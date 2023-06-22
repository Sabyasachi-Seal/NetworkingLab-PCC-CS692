#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

pthread_t r, w;

#define sip "127.0.0.1"
#define sp 1111
#define cip "127.0.0.10"
#define cp 2222
void* sndP(void* args){
	sleep(1);
	int cs = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(sip);
	saddr.sin_port = htons(sp);

	caddr.sin_family = AF_INET;
	caddr.sin_addr.s_addr = inet_addr(cip);
	caddr.sin_port = htons(cp);

	bind(cs, (struct sockaddr *)&caddr, sizeof(caddr));

	while(1){
	
		char buff[100];

		fgets(buff, 100, stdin);
		buff[strlen(buff)-1] = '\0';

		int len = sizeof(saddr);
		sendto(cs, buff, 100, 0, (struct sockaddr *)&saddr, len);

		printf("Server says: %s\n", buff);
	
	}
	close(cs);

}
void* rcvP(void* args){

	int cs = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(sip);
	saddr.sin_port = htons(sp);

	caddr.sin_family = AF_INET;
	caddr.sin_addr.s_addr = inet_addr(cip);
	caddr.sin_port = htons(cp);

	bind(cs, (struct sockaddr *)&caddr, sizeof(caddr));

	while(1){
	
		char buff[100];

		int len = sizeof(caddr);
		recvfrom(cs, buff, 100, 0, (struct sockaddr *)&saddr, &len);

		printf("Server says: %s\n", buff);
	
	}
	close(cs);

}

void main(){
	
	pthread_create(&r, NULL, (void *)rcvP, NULL);
	pthread_create(&w, NULL, (void *)sndP, NULL);

	pthread_join(r, NULL);
	pthread_join(w, NULL);

}
