#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>

#define max 100

typedef struct messageq{
	long int m_type;
	char message[max];
}mq;

int main(){
	int msgid, n;

	msgid = msgget((key_t)1234, IPC_CREAT|0666);

	mq message;

	message.m_type = 1;

	char buff[max];

	int run = 1;

	while(run){
		
		msgrcv(msgid, (void*)&message , max, message.m_type, 0);

		printf("Sender sent = %s\n", message.message);

		if(strncmp(message.message, "stop", 4)==0) run = 0;

	
	}

	msgctl(msgid, IPC_RMID, 0);
}


