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
	char msg[max];
	char divisor[20];
}crc;

void main(){
	remove(path);

	// make the socket
	int ss = socket(AF_UNIX, SOCK_STREAM, 0);

	// now bind the socket
	// make the structure
	struct sockaddr_un saddr, caddr;
	saddr.sun_family = AF_UNIX;
	strcpy(saddr.sun_path, path);

	// now bind
	bind(ss, (struct sockaddr *)&saddr, sizeof(saddr));

	// make the listen queue and start listening
	listen(ss, 5);

	// now since the listen queue is done, we can make the new connection with the tepm socket
	int len = sizeof(saddr);
	int ts = accept(ss, (struct sockaddr *)&caddr, &len);

	while(1){
		//reading
		crc data;
		read(ts, (void *)&data, sizeof(data));
		printf("Message read from client\n");
		//printf("Message read: %s\n", data.msg);
		//printf("divisor read: %s\n", data.divisor);
		
		//processing
		if(strcmp(data.msg, "EXIT")==0 || strcmp(data.msg, "exit")==0)	//exit condition
			break;
		
		
		int divisor_len= strlen(data.divisor); // length of the divisor
		int msg_c = divisor_len; 
		    
		char temp_d[20]= "";
		strncat(temp_d, data.msg, divisor_len);
		    
	    	while(msg_c<=strlen(data.msg)){
			
			// making a remiander
			char r[20]= "";
			
			//doing xor and storing in remainder
			for(int i=0; i<divisor_len; i++){
			    if(temp_d[i]==data.divisor[i])
				r[i]= '0';
			    else
				r[i]= '1';
			}
			
			//removing starting zeroes from remainder and updating temp_d
			int pos; // getting the first position of 1
			for(pos=0; pos<divisor_len; pos++){
			    if(r[pos]=='1')
				break;
			}

			
			int start=0;
			while(pos<divisor_len){
			    temp_d[start++]= r[pos++];
			}
			
			
			
			//carrying bits from og msg if needed
			while(start<divisor_len){
			    temp_d[start++]= data.msg[msg_c++];
			    //printf("msg_c: %d", msg_c);
			}
		    }
		    
			
		    //updating the msg for sending back to client
		    int i=strlen(data.msg) - strlen(temp_d);
		    int j=0;
		    for(;i<strlen(data.msg); i++, j++){
			if(data.msg[i]==temp_d[j])
				data.msg[i]= '0';
			    else
				data.msg[i]= '1';
		    }
		    //printf("Final message is: %s\n", data.msg);
		    
		    //sending data back to client
		    write(ts, (void *)&data, sizeof(data));
		    printf("Message sent to client\n\n");
		
		
		sleep(1);		
	}
	close(ts);

}
