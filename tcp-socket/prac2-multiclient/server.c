#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define max 100
#define ip "127.0.0.1"
#define port 1234
#define N 5

void* pthBody(void* args){
	
	int ts = *((int *)args);
	
	while(1){
		char buff[max];

		read(ts, buff, max);
	
		printf("Data read from client: %s\n", buff);
		
		if(strncmp(buff, "exit", 4)==0) break;

		write(ts, buff, max);
	}

	close(ts);
}

void main(){
	int ss = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;

	saddr.sin_addr.s_addr = inet_addr(ip);
	saddr.sin_port = htons(port);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	listen(ss, 5);
	int i = 0;
	while(1){
		pthread_t th;
		int len = sizeof(caddr);
		int ts = accept(ss, (struct sockaddr *)&caddr, &len);
		pthread_create(&th, NULL, (void *)pthBody, (void*)&ts);
		printf("\nCreated Thread: %d\n", i++);
	}
}


