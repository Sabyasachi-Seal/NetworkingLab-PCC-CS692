#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>
#include <string.h>

#define max 100
#define ip "127.0.0.1"
#define port 1234

void main(){
	
	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ip);
	saddr.sin_port = htons(port);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
	
		char buff[max];

		int length = sizeof(caddr);
		recvfrom(ss, buff, max, 0, (struct sockaddr *)&caddr, &length);

		printf("Message from client: %s\n", buff);

		// here lets manipulate the data

		//first find the parity bits

		int m = strlen(buff); //size of message
		
		int p = 0;
		
		while(pow(2, p)<p+m+1){
			p++; // find out parity bits
		}
		
		int len = m+p; // the total length of the new message
		char finalMsg[len];
		int i;
		int buff_c = 0;

		for(i=0; i<len; i++){
			if(ceil(log2(len-i)) == floor(log2(len-i))){
				// this means len - i is a power of 2
				finalMsg[i] = 'p';
			}
			else{
				finalMsg[i] = buff[buff_c++];
			}
		}
		
		finalMsg[len] = '\0';

		int bitsCalc = 0;
		while(bitsCalc<p){
			
			int posi = pow(2, bitsCalc);
			int bitPosi = posi;
			int ones = 0;

			while(bitPosi<=len){
				
				int j = 0;
				while(j<posi){
					
					if(finalMsg[len-(bitPosi+j)]=='1') 
						ones++;
					j++;
				}

				bitPosi = bitPosi + 2*posi;

			}

			if((ones%2)==0){
				finalMsg[len-posi] = '0';
			}
			else{
				finalMsg[len-posi] = '1';
			}

			bitsCalc++;

		}

		sendto(ss, finalMsg, max, 0, (struct sockaddr *)&caddr, length);
		
	}
	close(ss);
}
