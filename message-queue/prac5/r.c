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

void main(){
	int msgid = msgget((key_t)1234, IPC_CREAT|0666);
	
	mq msg;
	msg.m_type = 1;

	while(1){
//		char buff[100];

//		printf("Enter the message: \n");
			
		msgrcv(msgid, (void *)&msg, 100, msg.m_type, 0);

		printf("Message: %s\n", msg.message);
	}
	msgctl(msgid, IPC_RMID, 0);

}
