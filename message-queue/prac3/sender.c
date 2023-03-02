#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define max 50

typedef struct msg{
	long int m_type;
	char message[max];
}mq;

int main(){

	int msgid, n;
	
	mq msg;
	
	msg.m_type = 1;

	msgid = msgget((key_t)1234, IPC_CREAT|0666);

	// get a buffer, copy from the buffer to the message

	char buff[max];

	while(1){
		
		printf("Enter the data to be send: \n");

		n = read(0, buff, max);

		strcpy(msg.message, buff);

		msgsnd(msgid, (void *)&msg, sizeof(msg), 0);

		if(strncmp(msg.message, "stop", 4)==0){
			break;
		}

	}
	
}
