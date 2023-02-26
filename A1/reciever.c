#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define sized 64

typedef struct msgbuf{
	long mtype;
        char mtext[sized];
}mq;

int main(){

	int mqid=msgget(1234, 0666|IPC_CREAT);
	mq msg;
	msgrcv(mqid, (void *)&msg, sized, 1, 0);
	printf("Binary Reciever recieved: %s\n",msg.mtext);
	return 0;
}
