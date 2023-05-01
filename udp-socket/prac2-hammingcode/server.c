#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define max 100
#define ip "127.0.0.1"
#define port 1234

void main(){
	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ip);
	saddr.sin_port = port;

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
		char buff[max];

		int length = sizeof(caddr);
		recvfrom(ss, buff, max, 0, (struct sockaddr *)&caddr, &length);

		printf("Message from client: %s\n", buff);
		
		if(strncmp(buff, "exit", 4)==0) break;

		//here we can manipulate the data
		//first find the number of parity bits

		int p = 0;
		int m = strlen(buff);
		while(pow(2, p)<m+p+1){
			p++;
		}
		// therefore p is the number of parity bits
		//
		int len = m+p; // total len of message
		
		char finalMsg[len];
		int msgC = 0; // counter for the original msg
		int i=0;
		while(i<len){
			if(ceil(log2(len-i))==floor(log2(len-i))){
				finalMsg[i] = 'p';		
			}
			else{
				finalMsg[i] = buff[msgC];
				msgC++;
			}
			i++;
		}	
		// now calculate the parity bits
		int bitsCalc = 0;
		while(bitsCalc < p){
			int parityPosi = pow(2, bitsCalc);
			int tempPosi = parityPosi;
			int ones = 0;
			while(tempPosi <= len){
				int adjPosi = 0;
				while(adjPosi<parityPosi){
					if(finalMsg[len-(adjPosi + tempPosi)] == '1'){
						ones++;
					}
					adjPosi++;
				}
				tempPosi = tempPosi + 2*parityPosi;	
			}

			if((ones%2)==0) finalMsg[len-parityPosi] = '0';
			else finalMsg[len-parityPosi] = '1';
			bitsCalc++;
		}
		finalMsg[len] = '\0';
		sendto(ss, finalMsg, max, 0, (struct sockaddr *)&caddr, length);

	}
	close(ss);
}
