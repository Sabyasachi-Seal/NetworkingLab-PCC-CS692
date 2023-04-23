/*

broadcast.c

The following program sends or receives broadcast packets.
If invoked with one argument, it sends a packet containing
the current time to the local network broadcast IP and UDP port.
If invoked with no arguments, it receives and prints these packets.
Start it as a sender on just one host and as a receiver
on all the other hosts.
"192.168.203.255"
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include<string.h>

#define BROADCAST_PORT   4320
#define BROADCAST_IP     "255.255.255.255"

int
main (int argc)
{
  struct sockaddr_in addr;
  int addrlen, sock, cnt, reuse, allowBroadcast;
  struct ip_mreq mreq;
  char message[50];

/* set up socket */
  sock = socket (AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
    {
      perror ("socket");
      return (1);
    }

/* Enable SO_REUSEADDR to allow multiple instances of this application */
/* to receive copies of the broadcast datagrams using the same address. */
  reuse = 1;
  if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse)) < 0)
    {
      perror ("setsockopt reuse");
      return (1);
    }

  bzero ((char *) &addr, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl (INADDR_ANY);
  addr.sin_port = htons (BROADCAST_PORT);
  addrlen = sizeof (addr);

  if (argc > 1)			// Send
    {
/* Set socket to allow broadcast */
      allowBroadcast = 1;
      if (setsockopt
	  (sock, SOL_SOCKET, SO_BROADCAST, (void *) &allowBroadcast,
	   sizeof (allowBroadcast)) < 0)
	{
	  perror ("setsockopt allowBroadcast");
	  return (1);
	}

      addr.sin_addr.s_addr = inet_addr (BROADCAST_IP);

      while (1)
	{
	  //time_t t = time (0);
	  //sprintf (message, "time is %-24.24s", ctime (&t));
	  printf("Enter Message: ");
 	  fgets(message, 50, stdin);
	  message[strlen(message)-1]= '\0';
	  
	  if(strcmp(message, "QUIT")==0){
	  	strcpy(message, "\nThe Broadcast has ended :/\n");
	  	sendto (sock, message, sizeof (message), 0, (struct sockaddr *) &addr, addrlen);
	  	printf("\nEnding the Broadcast\n");
	  	break;
	  }
	  	
	  printf ("Sending to %s:  %s\n", BROADCAST_IP, message);
	  cnt =
	    sendto (sock, message, sizeof (message), 0,
		    (struct sockaddr *) &addr, addrlen);
	  if (cnt < 0)
	    {
	      perror ("sendto");
	      return (1);
	    }
	  sleep (5);		/* Avoids flooding the network */
	}
    }
  else				// Receive
    {
/* Bind socket to the broadcast port */
      if (bind (sock, (struct sockaddr *) &addr, sizeof (addr)) < 0)
	{
	  perror ("bind");
	  return (1);
	}

      while (1)
	{
	  cnt =
	    recvfrom (sock, message, sizeof (message), 0,
		      (struct sockaddr *) &addr, &addrlen);
	  if (cnt < 0)
	    {
	      perror ("recvfrom");
	      return (1);
	    }
	  else if (cnt == 0)
	    {
	      break;
	    }
	    
	  
	  printf ("Received from %s:  \"%s\"\n", inet_ntoa (addr.sin_addr),
		  message);
		  
	  if(strcmp(message, "\nThe Broadcast has ended :/\n")==0)
	  	break;
	}
    }
}
