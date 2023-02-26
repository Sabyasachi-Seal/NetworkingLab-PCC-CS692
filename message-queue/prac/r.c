#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


struct msg{
	long int msg_type;
	char txt[BUFSIZ];
};

int main(){
	int run  = 1;
	
	int msgid;

	struct msg newmsg;

	msgid = msgget((key_t)12345, 0666|IPC_CREAT);

	long int p = 0;

	while(run){
		msgrcv(msgid, (void*)&newmsg, BUFSIZ, p, 0);

		printf("\nData recived: %s", newmsg.txt);

		if(strncmp(newmsg.txt, "end", 3)==0){
			run = 0;
		}
	}

	msgctl(msgid, IPC_RMID, 0);
}
