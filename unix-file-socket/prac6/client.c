#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>

#define path "mysock"

#define max 100

typedef struct crcData{
	char divisor[20];
	char message[max];
}DATA;

void main(){
	
	int cs = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un saddr, caddr;
	saddr.sun_family = AF_UNIX;
	strcpy(saddr.sun_path, path);

	connect(cs, (struct sockaddr *)&saddr, sizeof(saddr));

	while(1){
		DATA data;
		
		printf("Enter divisor: \n");
		fgets(data.divisor, sizeof(data.divisor), stdin);
		data.divisor[strlen(data.divisor)-1] = '\0';

		printf("Enter message: \n");
		fgets(data.message, sizeof(data.message), stdin);
		data.message[strlen(data.message)-1] = '\0';

		int maxCoeff = 0;
		printf("Enter the highest coefficient: \n");
		scanf("%d", &maxCoeff);
		getchar();	
		int i;
		char ch = '0';
		for(i=0; i<maxCoeff; i++){
			strncat(data.message, &ch, 1);
		}

		write(cs, (void *)&data, sizeof(data));

		sleep(1);

		read(cs, (void *)&data, sizeof(data));

		printf("The final message is: %s\n", data.message);

	}

}
