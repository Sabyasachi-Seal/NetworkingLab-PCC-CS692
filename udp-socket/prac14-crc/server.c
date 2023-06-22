#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <math.h>

typedef struct crcData{
	char msg[100];
	char div[20];	
}crc;

void main(){

	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(1234);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));


	while(1){
	
		crc data;

		int leng = sizeof(caddr);
		recvfrom(ss, &data, sizeof(data), 0, (struct sockaddr *)&caddr, &leng);

		printf("Client says: %s\n",data.msg);

		// manipulate the data;
		//
		int divLen = strlen(data.div);
		int msgC = divLen;
		char temp[20] = "";

		strncat(temp, data.msg, divLen);

		int msgLen = strlen(data.msg);

		while(msgC <= msgLen){
			
			char rem[20] = "";
			int i = 0;
			for(;i<divLen;i++){
				if(data.div[i] == temp[i]){
					rem[i] = '0';
				}
				else{
					rem[i] = '1';
				}
			}

			// now you need to find the posi for the first one
			int posi = 0;
			while(posi<divLen){
				if(rem[posi] == '1'){
					break;
				}
				posi++;
			}

			// copy the right of the first one to the temp 
			int i2 = 0;
			while(posi < divLen){
				temp[i2++] = rem[posi++];
			}

			// now copy some from the msg to the temp
			while(i2<divLen){
				temp[i2++] = data.msg[msgC++];
			}
		//	printf("In loop 1\n");

		}
		int start = strlen(data.msg) - strlen(temp);
		int j = 0;
		for(;start<strlen(data.msg);start++){
			if(data.msg[start] == temp[j]){
				data.msg[start] = '0';
			}
			else{
				data.msg[start] = '1';
			}
			j++;
		}
		printf("Sending message %s\n", data.msg);		
		sendto(ss, &data, 100, 0, (struct sockaddr *)&caddr, leng);
	
	}

}
