#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define max 100
#define path "127.0.0.1"

typedef struct crcd{
	char msg[100];
	char div[20];
}crc;

void main(){
	int cs = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr, caddr;
	saddr.sin_family = AF_INET;
//	strcpy(saddr.sun_path, path);
	
	saddr.sin_addr.s_addr = inet_addr(path);
	saddr.sin_port = 1234;

	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
		
		crc data;

		printf("Enter the message: \n");
		read (0, data.msg, max);
		data.msg[strlen(data.msg)-1] = '\0';


		printf("Enter the maximum power of the polnomial: ");
		int n;
		scanf("%d", &n);
		getchar();

		printf("Enter the powers: \n");
		read(0, data.div, sizeof(data.div));
		data.div[strlen(data.div)-1] = '\0';

		// now we need to add n number of free bits to the data
		char ch = '0';
		int i=0;
		for(;i<n; i++){
			strncat(data.msg, &ch, 1);
		}

		write(cs, (void *)&data, sizeof(data));

		read(cs, (void *)&data, sizeof(data));

		printf("Client says: %s\n", data.msg);
	}
}
