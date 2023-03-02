#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define max sizeof(int)

typedef struct msg{
	long int m_type;
	char message[max+1];
}mq;

int main(){

	int msgid, n;
	
	mq msg;

	msg.m_type = 3;

	msgid = msgget((key_t)1234, 0666|IPC_CREAT);

	while(1){
		msgrcv(msgid, (void*)&msg, sizeof(msg), msg.m_type, 0);

		printf("Message Received = %s\n", msg.message);

	}

}
