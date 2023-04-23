#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>

#define path "mysock"

#define max 100

typedef struct crcData{
	char divisor[20];
	char message[max];
}DATA;

void main(){

	remove(path);

	int i;	
	int ss = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un saddr, caddr;
	saddr.sun_family = AF_UNIX;
	strcpy(saddr.sun_path, path);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	listen(ss, 5);

	int len = sizeof(caddr);
	int ts = accept(ss, (struct sockaddr *)&caddr, &len);

	while(1){
		DATA data;
		
		read(ts, (void *)&data, sizeof(data));

		printf("Message recieved: %s\n", data.message);
		printf("Divisor given: %s\n", data.divisor);

		 // checks which point to check for the next non-taken char
		int div_len = strlen(data.divisor);
		int msg_c = div_len;
		char temp[20] = ""; // used to store the message bits taken

		strncat(temp, data.message, div_len);
		// this means the fist div len number of elements are taken
		// so we can set msg counter to div_len

//		msg_c = div_len;

		while(msg_c <= strlen(data.message)){
			
			char r[20] = ""; // used to store the remainder
			
			// this will find the remainder of the xor
			for(i=0; i<div_len; i++){
				if(data.divisor[i] == temp[i]) 
					r[i] = '0';
				else 
					r[i] = '1';
			}

			printf("%s\n", r);
			// now find the position where the first one is
			int pos;
			for(pos=0; pos<div_len; pos++){
				if(r[pos] == '1'){
					break;
				}
			}

			// this means these are the new first digits of the temp, so lets recreate temp;
			i = 0;
			while(pos<div_len){ // we check the pos since the number of 0 we are limited on the data in the remainder, and we are taking from the first 1
				temp[i++] =r[pos++];
			}

			// now we need to copy some data from the original message to the temp to make the temp size equal to div_len
			while(i<div_len){
				temp[i++] = data.message[msg_c++];
			}
			// this will continue until the last of the message bits have been read				
		}
		
		
		// now lets copy the new remaining into the original message to be sent to the client
		i = strlen(data.message) - strlen(temp);
		int j = 0;

		for(;i<strlen(data.message); i++, j++){
			if(data.message[i] == temp[j]){
				data.message[i] = '0';
			}
			else data.message[i] = '1'; 
		}

		write(ss, (void *)&data, sizeof(data));

		printf("The final message is: %s\n", data.message);
		sleep(1);
	}

}

