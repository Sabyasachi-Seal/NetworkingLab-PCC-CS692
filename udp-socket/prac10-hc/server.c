#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <math.h>


void main(){

	int ss = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(1234);

	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
	
		char buff[100];

		int leng = sizeof(caddr);
		recvfrom(ss, buff, 100, 0, (struct sockaddr *)&caddr, &leng);

		printf("Client says: %s\n",buff);


		int p =0;
		int m = strlen(buff);

		while(pow(2, p)<m+p+1){
			p++;
		}

		int len = m+p;
		char msg[len];
		int msgC = 0;

		int i = 0;
		while(i<len){
			if(ceil(log2(len-i)) == floor(log2(len-i))){
				msg[i] = 'p';
			}
			else{
				msg[i] = buff[msgC++];
			}
			i++;
		}

		int bitsPlaced = 0;
		while(bitsPlaced<p){
			int parityPosi = pow(2, bitsPlaced);
			int tempPosi = parityPosi;
			int ones = 0;

			while(tempPosi <= len){

				int adjPosi = 0;
				while(adjPosi<parityPosi){
					if(msg[len-(adjPosi + tempPosi)] == '1'){
						ones++;
					}
					adjPosi++;					
				}
				tempPosi = tempPosi + 2*parityPosi;

			}
			bitsPlaced++;
			if((ones%2)==0){
				msg[len - parityPosi] = '0';
			}
			else{
			
				msg[len - parityPosi] = '1';
			}
		}
		msg[len] = '\0';

		sendto(ss, msg, 100, 0, (struct sockaddr *)&caddr, leng);
	
	}

}
