#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>


#define max 100

typedef struct message{
	long int m_type;
	char message[max];
}mq;

int main(){
	int run, msgid, n;
	run = 1;
	msgid = msgget((key_t)1234, IPC_CREAT|0666);

	mq message;

	char buff[max];

	while(run){

		printf("Enter message :\n");

		fgets(buff, max, stdin);

		message.m_type = 1;

		strcpy(message.message, buff);

		msgsnd(msgid, (void *)&message, max, 0);

		//printf("%c", &strncpy(message.message, "stop", 4));

		if(strncmp(message.message, "stop", 4)==0){
			//printf("STOP\n");
			run = 0;
		}	

	}

}
