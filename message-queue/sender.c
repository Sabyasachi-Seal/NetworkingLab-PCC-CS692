#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MAX_TEXT 50

struct my_msg{
	long int msg_type;
	char some_text[MAX_TEXT];
};

int main(){
	int running=1; // for controlling while loop
	int msgid;
	struct my_msg some_data;
	char buffer[100];
	msgid=msgget((key_t)12345, 0666|IPC_CREAT);
	if(msgid==-1){
		printf("Error in making queue");
	}
	while(running){
		printf("Enter some data: \n");
		fgets(buffer, 100, stdin);
		some_data.msg_type = 1;
		strcpy(some_data.some_text, buffer);
		if(msgsnd(msgid, (void*)&some_data, MAX_TEXT, 0)==-1){
			printf("msg not sent\n");
		}
		if(strncmp(buffer, "end", 3)==0){
			running = 0;
		}
		
	}
}
