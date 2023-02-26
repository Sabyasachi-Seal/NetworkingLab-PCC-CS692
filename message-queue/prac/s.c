#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define max 1024

struct msg{
	long int msg_type;
	char text[max];
};

int main(){
	int run = 1;

	int msgid; 

	struct msg newmsg;

	char buffer[max];

	msgid = msgget((key_t)12345, 0666|IPC_CREAT);

	while(run){
		printf("Enter data to be sent: \n");

		read(0, buffer, max);

		strcpy(newmsg.text, buffer);

		msgsnd(msgid, (void*)&newmsg, max, 0);

		if(strncmp(buffer, "end", 3)==0){
			run = 0;
		}
	}
}
