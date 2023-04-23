#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define max sizeof(int)

typedef struct mQueue{
	long int mType;
	char buffer[max+1];
}mQ;

void main(){
	int msgId;

	mQ newMessage;
	
	// msg id made
	msgId = msgget(6767, 0666|IPC_CREAT);	

	int i;

	while(1){
		i = 0;

		msgrcv(msgId, (void *)&newMessage, max,1, 0);

//		strcpy(buff, newMessage.buffer);

		printf("Recieved from sender: %s\n", newMessage.buffer);

		if(strncmp(newMessage.buffer, "0000", 4)==0) break;
		
	//	while(i++<max){
	//		buff[i] = '\0';		
	//	}

	}
	msgctl(msgId, IPC_RMID, NULL);

}
