#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


int main (int ac, char **av){
  int i, j, l, n, sid, cid, cnt;
  struct sockaddr_in saddr, caddr;
  char sip_addr[32], t, msg[32], res[32];

  if(ac == 1)
    strcpy(sip_addr, "127.0.0.1");
  else
    strcpy(sip_addr, av[1]);
  sid = socket(AF_INET, SOCK_STREAM, 0);
  saddr.sin_family = AF_INET;
  //sad.sin_addr.s_addr = inet_addr (sip_addr);
  inet_aton(sip_addr, &(saddr.sin_addr));
  saddr.sin_port = htons(1234);
  bind(sid, (struct sockaddr *)&saddr, sizeof(saddr));
  listen(sid, 1);
  l = sizeof(caddr);
  cid = accept(sid, (struct sockaddr *)&caddr, &l);

  while(1){
      n = read(cid, (void *)msg, sizeof(msg));
      msg[n] = 0;
      if(!strcmp(msg, "EXIT") || !strcmp(msg, "exit"))
	break;

      // Bit-stuffing and resending the message
      int i=0, j=0;
	while(msg[i]!='\0'){
		if(msg[i]=='1'){
			cnt=1;
			res[j]=msg[i];
			for(int k=i+1; msg[k]=='1' && k<n && cnt<5; k++){
				j++;
				res[j]=msg[k];
				cnt++;
				if(cnt==5){
					j++;
					res[j]='0';
				}
				i=k;
			}
		}
		else
			res[j]=msg[i];
		i++;
		j++;
	}
	res[j]='\0';
      write(cid, (void *)res, sizeof(res));
  }
  return 0;
}


