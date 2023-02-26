#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define sized sizeof(int)

typedef struct msgbuf{
	long mtype;
        char mtext[sized+1];
}mq;

void bin(int x,mq* msg){
	int cnt=sized;
	msg->mtext[cnt]='\0';
	cnt-=1;
	while(cnt>=0){
		msg->mtext[cnt]=(x&1)+'0';
        x=x>>1;
		cnt-=1;
	}
}

void oct(int x,mq* msg){
	int cnt=sized;
	msg->mtext[cnt]='\0';
	cnt-=1;
	while(cnt>=0){
		msg->mtext[cnt]=(x%8)+'0';
        x=x/8;
		cnt-=1;
	}
}

void hex(int x,mq* msg){
	int cnt=sized;
	msg->mtext[cnt]='\0';
	cnt-=1;
	while(cnt>=0){
	    int temp=x%16;
	    if(temp<10)
    		msg->mtext[cnt]=temp+'0';
    	else
    	    msg->mtext[cnt]=temp+55;
        x=x/16;
		cnt-=1;
	}
}
int main(){

	int mqid=msgget(1234, 0666|IPC_CREAT);
	mq msb,mso,msh;
	msb.mtype=1;
	mso.mtype=2;
	msh.mtype=3;
	int x;
	printf("Enter a number: ");
	scanf("%d",&x);
	bin(x,&msb);
	oct(x,&mso);
	hex(x,&msh);
	printf("\nsending binary: %s\n",msb.mtext);
	printf("sending octal: %s\n",mso.mtext);
	printf("sending hexadecimal: %s\n",msh.mtext);
	msgsnd(mqid, (void *)&msb, sized, 0);
	msgsnd(mqid, (void *)&mso, sized, 0);
	msgsnd(mqid, (void *)&msh, sized, 0);
	
	return 0;
}
