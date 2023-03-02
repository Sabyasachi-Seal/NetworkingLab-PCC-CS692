#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define max 100
#define path "127.0.0.1"

typedef struct crcd{
	char msg[100];
	char div[20];
}crc;

void main(){
	remove(path);

	int ss = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
//	strcpy(saddr.sun_path, path);
	saddr.sin_addr.s_addr = inet_addr(path);
	saddr.sin_port = 1234;
	

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	listen(ss, 5);

	int len = sizeof(caddr);
	int ts = accept(ss, (struct sockaddr *)&caddr, &len);

	while(1){
		
		crc data;

		read(ts, (void *)&data, sizeof(data));

		printf("Client says: %s\n", data.msg);

		int div_len = strlen(data.div);
		int mcount = div_len;

		// first copy the first few bits into the temp data
	
		char tempd[20] = "";

		strncat(tempd, data.msg, div_len);

		while(mcount<=strlen(data.msg)){

			char r[20] = "";

			int i;
			
			// initial xor
			for(i=0; i<div_len; i++){
				if(tempd[i] == data.msg[i]){
					r[i] = '0';
				}
				else{
					r[i] = '1';
				}
			}

			// get the position of the first 1
			int pos;
			for(pos=0; pos<div_len; pos++){
				if(r[pos] == '1'){
					break;
				}
			}
			
			// now this is the new bits for the tempd, so copy them
			int start;
			for(start=0; pos<div_len; pos++, start++){
				tempd[start] = r[pos]; 
			}

			//now take some new bits for the tempd
			for(;start<div_len; start++, mcount++){
				tempd[start] = data.msg[mcount];
			}

		}


		// copy the new coded bits into original msg
		int i = strlen(data.msg) - strlen(tempd);
		int j = 0;
		for(;i<strlen(data.msg); i++, j++){
			if(data.msg[i] == tempd[j]){
				data.msg[i] = '0';
			}
			else{
				data.msg[i] = '1';
			}
		}

		write(ts, (void *)&data, sizeof(data));
	}
	close(ts);
}
