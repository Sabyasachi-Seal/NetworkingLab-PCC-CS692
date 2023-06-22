#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void main(){
	int ss = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(1234);
	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));
	listen(ss, 5);
	int len = sizeof(caddr);
	int ts = accept(ss, (struct sockaddr *)&caddr, &len);
	while(1){
		char buff[100];
		read(ts, buff, 100);
		printf("Client says: %s\n",buff);

		// we can manipulate the data here
		//
		int i;
		int bitsLoc[20], currOnes=0, totalOnes=0;
		for(i=0; i<strlen(buff); i++){
			if(buff[i]=='1'){
				currOnes++;
				if(currOnes == 5){
					bitsLoc[totalOnes++] = i+1;
					currOnes = 0;
				}
			}
			else{
				currOnes = 0;
			}
		}

		int bitsPlaced = 0;
		while(bitsPlaced < totalOnes){
			int currBitLoc = bitsLoc[bitsPlaced];

			int j;
			for(j = strlen(buff); j>=currBitLoc; j--){
				buff[j+1] = buff[j];
			}

			buff[currBitLoc + bitsPlaced] = '0';
			bitsPlaced++;
		}

		write(ts, buff, 100);
	}
	close(ts);
}
