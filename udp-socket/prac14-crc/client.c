#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

typedef struct crcData{
	char msg[100];
	char div[20];
}crc;

void main(){

	int cs = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(1234);

	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	int len = sizeof(saddr);
	while(1){
	
		crc data;

		int n;

		printf("Enter the max bits for the power: \n");
		scanf("%d", &n);
		getchar();

		printf("Enter the divisior: \n");
		fgets(data.div, 20, stdin);
		data.div[strlen(data.div)-1] = '\0';

		printf("Enter the message: \n");
		fgets(data.msg, 100, stdin);
		data.msg[strlen(data.msg)-1] = '\0';

	 	char ch = '0';
		while(n>0){
			strncat(data.msg, &ch, 1);
			n--;
		}

		sendto(cs, &data, sizeof(data), 0, (struct sockaddr *)&saddr, len);
		
		recvfrom(cs, &data, sizeof(data), 0, (struct sockaddr *)&saddr, &len);

		printf("Server says: %s\n", data.msg);

	
	}

}
