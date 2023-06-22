#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>

#define add "127.0.0.1"

sem_t s;
pthread_t t3;
int timeOut=0, count=0, seq;

void* timer(void* args){
	count++;
	printf("Timer started %d\n", count);
	sleep(5);
	timeOut = 1;
	sem_post(&s);
	pthread_exit(NULL);
}

void* sender(void* args){

	int cs = *((int *)args);

	while(1){
		if(!timeOut){
		
			write(cs, &seq, sizeof(seq));
			printf("Seq No: %d\n", seq);
			seq = (seq+1)%2;
			
		}
		else if(timeOut){
			seq = (seq+1)%2;
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

void* receive(void* args){

	int cs = *((int *)args);

	int ack;
	while(1){
		read(cs, &ack, sizeof(ack));
		if(ack == seq){
			pthread_cancel(t3);
			timeOut = 0;
			sem_post(&s);
		}
	}
	pthread_exit(NULL);

}

void main(){
	
	int cs = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr,caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(add);
	saddr.sin_port = htons(1234);

	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	pthread_t t1, t2;
	
	sem_init(&s, 0, 1);

	pthread_create(&t1, NULL, sender, &cs);
	pthread_create(&t2, NULL, receive, &cs);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);	

}

