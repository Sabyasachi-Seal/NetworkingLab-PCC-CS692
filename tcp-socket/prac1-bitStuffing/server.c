#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define max 100
#define path "127.0.0.10"

void main(){
	
	int ss = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(path);
	saddr.sin_port=htons(1234);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	listen(ss, 5);
	
	int len = sizeof(caddr);
	int ts = accept(ss, (struct sockaddr *)&caddr, &len);
	
	int i;
	while(1){
		
		char buff[max];

		read(ts, (void *)&buff, max);
	
		printf("Message from client: %s\n", buff);	
		
		if(strncmp(buff, "exit", 4)==0) break;

		// now manipulate the data
		int bitLoc[20],  bitsCount=0, currOnes=0;
		for(i=0; i<strlen(buff); i++){
			if(buff[i] == '1'){				
				currOnes++;
				if(currOnes==5){
					bitLoc[bitsCount++] = i+1;
					currOnes = 0;	
				}
			}
			else{
				currOnes = 0;
			}
		}

		int bitsPlaced = 0;

		// now we need to place the bits in those index
		while(bitsPlaced<bitsCount){
			
			int currBitLoc = bitLoc[bitsPlaced];

			// now shift the remaining to the back to put the bit;
			int j;
			for(j=strlen(buff); j>=currBitLoc; j--){
				buff[j+1] = buff[j];
			}

			buff[currBitLoc+bitsPlaced] = '0';
			bitsPlaced++;

		}

		write(ts, (void *)&buff, max);

		sleep(1);

	}
	close(ts);


}
