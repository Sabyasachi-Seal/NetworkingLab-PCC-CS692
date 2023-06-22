#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>
#include <string.h>

#define ip "127.0.0.1"
#define port 1234

void main(){

	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ip);
	saddr.sin_port = htons(port);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
	
		char buff[100];

		int leng = sizeof(caddr);
		recvfrom(ss, buff, 100, 0, (struct sockaddr *)&caddr, &leng);

		printf("Client says: %s\n", buff);

		int p = 0;
		int m = strlen(buff);
		while(pow(2, p)<m+p+1){
			p++;
		}

		int len = m+p;

		char finalMsg[len];
		int msgC = 0;
		int i = 0;

		int buffC = 0;

		while(i<len){
			if(ceil(log2(len-i)) == floor(log2(len-i))){
				finalMsg[i] = 'p';
			}
			else{
				finalMsg[i] = buff[buffC++];
			}
			i++;
		}

		int bitsCalc = 0;
		while(bitsCalc < p){
		
			int parityPosi = pow(2, bitsCalc);
			int tempPosi = parityPosi;
			int ones = 0;

			while(tempPosi <= len){
				
				int adjPosi = 0;
				while(adjPosi < parityPosi){
					
					if(finalMsg[len-(tempPosi + adjPosi)] == '1'){
					
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

		sendto(ss, finalMsg, 100, 0, (struct sockaddr *)&caddr, sizeof(caddr));

	}
}
