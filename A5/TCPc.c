#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


int main (int ac, char **av){
  struct sockaddr_in saddr;
  int sid, i, j, n;
  char sip_addr[32], msg[32], t;

  if(ac == 1)
    strcpy(sip_addr, "127.0.0.1");
  else
    strcpy(sip_addr, av[1]);

  sid = socket(AF_INET, SOCK_STREAM, 0);
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr(sip_addr);
  saddr.sin_port = htons(1234);
  connect (sid, (struct sockaddr *)&saddr, sizeof(saddr));

  while(1){
    printf("Enter the message data to be sent: ");
    fgets (msg, sizeof(msg), stdin);
	msg[strlen(msg)-1]=0;
    write(sid, (void *)msg, sizeof(msg));
    if(!strcmp(msg, "EXIT") || !strcmp(msg, "exit"))
		break;
    n = read(sid, (void *)msg, sizeof(msg));
    msg[n] = 0;
    printf("\nBit-stuffed data returned from server: %s\n\n", msg);
  }
  close(sid);
  return 0;
}

