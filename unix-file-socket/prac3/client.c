#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define path "mysock"
#define max 100

typedef struct crcData{
	char msg[100];
	char divisor[20];
}crc;

void main(){
	// make the socket
	int cs = socket(AF_UNIX, SOCK_STREAM, 0);

	// now bind the socket
	// make the structure
	struct sockaddr_un saddr, caddr;
	saddr.sun_family = AF_UNIX;
	strcpy(saddr.sun_path, path);

	// now connect
	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
//		char buff[max];
		
		crc data;

		printf("Enter some message: \n");

		read(0, data.msg, max);

		data.msg[strlen(data.msg)-1] = '\0';

//		int n = read(0, buff, max);

//		buff[n-1] = '\0';

		int x = 0;
		printf("Enter the max power of X : \n");
		scanf("%d", &x);
		getchar();
		
		printf("Enter the divisor: \n");
		read(0, data.divisor, sizeof(data.divisor));
		data.divisor[strlen(data.divisor)-1] = '\0';

		// now we need to add the additional bits to the msg
		char ch = '0';
		for(int i=0; i<x; i++){
			strncat(data.msg, &ch, 1);
		}

		write(cs, (void *)&data, sizeof(data));

//		if(strncmp(buff, "exit", 4)==0) break;
		
		read(cs, (void *)&data, sizeof(data));

//		n = strlen(buff);

//		buff[n] = '\0';

		printf("Message from server: %s\n", data.msg);
	}

}
