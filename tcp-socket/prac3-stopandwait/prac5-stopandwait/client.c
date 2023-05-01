#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

pthread_t t3;
sem_t s;
int seqno=0, timedout =0;

void* timer(){

	sleep(5);
	timedout = 1;
	sem_post(&s);
	pthread_exit(NULL);

}

void* sender(void* args){

	int cs = *((int *)args);

	while(1){
		if(!timedout){
			printf("Send the next\n");
			write(cs, &seqno, sizeof(seqno));
			seqno = (seqno+1)%2;
		}
		else{
		
			seqno = (seqno+1)%2;
			write(cs, &seqno, sizeof(seqno));

		}
		timedout = 0;
		pthread_create(&t3, NULL, timer, NULL);
		sem_wait(&s);
		pthread_join(t3, NULL);
	}
	pthread_exit(NULL);
}

void* receiver(void * args){
	int cs = *((int *)args);
	int ack;
	while(1){
		read(cs, &ack, sizeof(ack));
		if(ack == seqno){
			pthread_cancel(t3);
			timedout = 0;
			sem_post(&s);
		}
	}
	pthread_exit(NULL);
}

void main(){
	int cs = socket(AF_INET, SOCK_STREAM, 0);
	perror("socket");

	struct sockaddr_in c;
	c.sin_family = AF_INET;
	c.sin_port = htons(5555);
	inet_aton("127.0.0.1", &c.sin_addr);
	connect(cs,(struct sockaddr *)&c, sizeof(c));
	perror("connect");

	sem_init(&s, 0, 1);
	pthread_t t1, t2;
	pthread_create(&t1, NULL, sender, &cs);
	pthread_create(&t2, NULL, receiver, &cs);

	pthread_join(t1, 0);
	pthread_join(t2, 0);
}
