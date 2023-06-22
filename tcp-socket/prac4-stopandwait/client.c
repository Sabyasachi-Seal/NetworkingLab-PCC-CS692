#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>

#define add "127.0.0.1"

//---------------------------------------------------------------EVEN PARITY SERVER---------------------------------------------------------------------

sem_t s;
pthread_t t3;
int count=0, timeOut=0;
int seq;

void* timer(void* args){
	count++;
	printf("Timer started %d\n", count);
	sleep(5);
	timeOut = 1;
	sem_post(&s);
	pthread_exit(NULL);
}

void* sending(void* args){
	int cs = *((int*)args);

	while(1){
		if(!timeOut){
			write(cs, &seq, sizeof(seq));
			printf("Seqence no: %d\n", seq);
			seq=(seq+1)%2;
		}
		else if(timeOut){
			seq=(seq+1)%2;
			printf("Resending %d\n",seq);
			write(cs, &seq, sizeof(seq));
		}
		timeOut = 0;
		pthread_create(&t3, NULL, (void*)&timer, NULL);
		sem_wait(&s);
		pthread_join(t3, NULL);
	}
	pthread_exit(NULL);

}

void* receiving(void* args){
	int cs = *((int *)args);

	int ack;
	while(1){
		read(cs, (void*)&ack, sizeof(ack));
		if(ack == seq){
			pthread_cancel(t3);
			timeOut= 0 ;
			sem_post(&s);
		}
	}
	pthread_exit(NULL);


}

void main(){
	//creating server socket ss
	int ss= socket(AF_INET, SOCK_STREAM, 0);
	
	//binding server address to ss
	
	//defining structure to store address of server 
	struct sockaddr_in saddr, caddr;
	saddr.sin_family= AF_INET;
	saddr.sin_addr.s_addr = inet_addr(add);
	saddr.sin_port = 1234;
	
	//binding
	connect(ss, (struct sockaddr *)&saddr, sizeof(saddr));
	
	sem_init(&s, 0, 1);
	pthread_t t1, t2;
	pthread_create(&t1, NULL, sending, &ss);
	pthread_create(&t2, NULL, receiving, &ss);	

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
