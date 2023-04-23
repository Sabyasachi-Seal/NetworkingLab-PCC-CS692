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

void bin(mQ* msg, int x){
	int ind = max;
	msg->buffer[ind] = '\0';
	ind--;
	while(ind>=0){
		msg->buffer[ind] = (x&1) + '0';
		x >>= 1;
		ind--;
	}
}
void oct(mQ* msg, int x){
	int ind = max;
	msg->buffer[ind] = '\0';
	ind--;
	while(ind>=0){
		msg->buffer[ind] = (x&7) + '0';
		x >>= 3;
		ind--;
	}
}
void hex(mQ* msg, int x){
	int ind = max;
	msg->buffer[ind] = '\0';
	ind--;
	while(ind>=0){
		
		int temp = (x&15);

		if(temp>=10) msg->buffer[ind] = temp + 55;

		else msg->buffer[ind] = temp + '0';

		x >>= 4;
		ind--;
	}
}
void main(){
	int msgId;
	// char buff[max];
	mQ msgB, msgO, msgH;
	
	// msg id made
	msgId = msgget(6767, 0666|IPC_CREAT);	

	int run = 1;

	while(run){
		int n;

		printf("Write something: \n");
		//read(0, buff, max)
		//strcpy(newMessage.buffer, buff);

		msgB.mType = 1;
		msgO.mType = 2;
		msgH.mType = 3;

		scanf("%d", &n);

		bin(&msgB, n);	
		oct(&msgO, n);
		hex(&msgH, n);

//		printf("%s", newMessage.buffer);

		msgsnd(msgId, (void *)&msgB, max, 0);
		msgsnd(msgId, (void *)&msgO, max, 0);
		msgsnd(msgId, (void *)&msgH, max, 0);

		if(n==0) break;
		
	//	int i = 0;
	//	while(i++<max){
	//		buff[i] = '\0';
	//	}

	}
	msgctl(msgId, IPC_RMID, NULL);

}
