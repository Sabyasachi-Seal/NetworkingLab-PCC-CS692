/*

multicast.c

The following program sends or receives multicast packets.
If invoked with one argument, it sends a packet containing
the current time to an arbitrarily chosen multicast group and UDP port.
If invoked with no arguments, it receives and prints these packets.
Start it as a sender on just one host and as a receiver on all the other hosts.

*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MULTICAST_PORT   4321
#define MULTICAST_GROUP  "224.0.0.1"

int
main (int argc)
{
  struct sockaddr_in addr;
  int addrlen, sock, cnt, reuse = 1;
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
/* to receive copies of the multicast datagrams using the same address. */
  if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse)) < 0)
    {
      perror ("setsockopt reuse");
      return (1);
    }

  bzero ((char *) &addr, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl (INADDR_ANY);
  addr.sin_port = htons (MULTICAST_PORT);
  addrlen = sizeof (addr);

  if (argc > 1)			// Send
    {
      addr.sin_addr.s_addr = inet_addr (MULTICAST_GROUP);

      while (1)
	{
	  //time_t t = time (0);
	printf("Enter Message: ");
	fgets(message, 50, stdin);
	message[strlen(message)-1]= '\0';
	  //sprintf (message, "time is %-24.24s", ctime (&t));
	  printf ("Sending to %s:  %s\n", MULTICAST_GROUP, message);
	  cnt =
	    sendto (sock, message, sizeof (message), 0,
		    (struct sockaddr *) &addr, addrlen);
	  if (cnt < 0)
	    {
	      perror ("sendto");
	      return (1);
	    }
	  sleep (5);
	}
    }
  else				// Receive
    {
/* Bind socket to the multicast port */
      if (bind (sock, (struct sockaddr *) &addr, sizeof (addr)) < 0)
	{
	  perror ("bind");
	  return (1);
	}
/* Set socket to join a multicast group */
      mreq.imr_multiaddr.s_addr = inet_addr (MULTICAST_GROUP);
      mreq.imr_interface.s_addr = htonl (INADDR_ANY);
      if (setsockopt
	  (sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof (mreq)) < 0)
	{
	  perror ("setsockopt ADD_MEMBERSHIP");
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
	}
    }
}
