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

void bin(int n, mq* msg){

	int ind = max;
	
	msg->message[ind] = '\0';
	
	ind-=1;

	while(ind>=0){
		msg->message[ind] = (n&1) + '0';
		n >>= 1;
		ind--;
	}
	
}
void oct(int n, mq* msg){

	int ind = max - 1;
	
	msg->message[max] = '\0';

	while(ind>=0){
		msg->message[ind] = (n%8) + '0';
		n>>=3;
		ind--;
	}
	
}
void hex(int n, mq* msg){

	int ind = max - 1;
	
	msg->message[max] = '\0';

	while(ind>=0){
		int t = n%16;
		if(t < 10){
			msg->message[ind] = t + '0';
		}
		else{
			msg->message[ind] = t + 55;
		}
		n>>=4;
		ind--;
	}
	
}
int main(){

	int msgid, n;
	
	mq msgb, msgo, msgh;
	
	msgb.m_type = 1;
	msgo.m_type = 2;
	msgh.m_type = 3;

	msgid = msgget((key_t)1234, IPC_CREAT|0666);

	while(1){
		printf("Enter the data to be send: \n");

		int n;

		scanf("%d", &n);

		bin(n, &msgb);
		oct(n, &msgo);
		hex(n, &msgh);

		msgsnd(msgid, (void *)&msgb, max, 0);

		msgsnd(msgid, (void *)&msgo, max, 0);

		msgsnd(msgid, (void *)&msgh, max, 0);

		if(n==-1){
			break;
		}

	}
	
}
