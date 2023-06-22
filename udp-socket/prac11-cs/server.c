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
	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(sip);
	saddr.sin_port = htons(sp);

	caddr.sin_family = AF_INET;
	caddr.sin_addr.s_addr = inet_addr(cip);
	caddr.sin_port = htons(cp);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
	
		char buff[100];

		fgets(buff, 100, stdin);
		buff[strlen(buff)-1] = '\0';

		int len = sizeof(caddr);
		sendto(ss, buff, 100, 0, (struct sockaddr *)&caddr, len);

		printf("Server says: %s\n", buff);
	
	}
	close(ss);

}
void* rcvP(void* args){

	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(sip);
	saddr.sin_port = htons(sp);

	caddr.sin_family = AF_INET;
	caddr.sin_addr.s_addr = inet_addr(cip);
	caddr.sin_port = htons(cp);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
	
		char buff[100];

		int len = sizeof(caddr);
		recvfrom(ss, buff, 100, 0, (struct sockaddr *)&caddr, &len);

		printf("Server says: %s\n", buff);
	
	}
	close(ss);

}

void main(){
	
	pthread_create(&r, NULL, (void *)rcvP, NULL);
	pthread_create(&w, NULL, (void *)sndP, NULL);

	pthread_join(r, NULL);
	pthread_join(w, NULL);

}
