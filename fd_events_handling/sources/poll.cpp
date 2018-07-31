/*
 * Poll vs Select
 * poll( ) does not require that the user calculate the value of the highest- numbered file descriptor +1
 * poll( ) is more efficient for large-valued file descriptors. Imagine watching a single file descriptor with the value 900 via select()—the kernel would have to check each bit of each passed-in set, up to the 900th bit.
 * select( )’s file descriptor sets are statically sized.
 * With select( ), the file descriptor sets are reconstructed on return, so each subsequent call must reinitialize them. The poll( ) system call separates the input (events field) from the output (revents field), allowing the array to be reused without change.
 * The timeout parameter to select( ) is undefined on return. Portable code needs to reinitialize it. This is not an issue with pselect( )
 * select( ) is more portable, as some Unix systems do not support poll( )
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>//srandom
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>//strlen
#define MAXBUF 256

static void child_process(void)
{
  sleep(2);
  char msg[MAXBUF];
  struct sockaddr_in addr = {0};
  int n, sockfd,num=1;
  srandom(getpid());
  /* Create socket and connect to server */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

  printf("child {%d} connected \n", getpid());
  while(1){
        int sl = (random() % 10 ) +  1;
        num++;
     	sleep(sl);
  	sprintf (msg, "Test message %d from client %d", num, getpid());
  	n = write(sockfd, msg, strlen(msg));	/* Send message */
  }

}
int poll_main()
{
  char buffer[MAXBUF];
  struct sockaddr_in addr;
  struct sockaddr_in client;
  int addrlen, i;
  pollfd pollfds[10];
  int sockfd;
  for(i=0;i<5;i++)
  {
  	if(fork() == 0)
  	{
  		child_process();
  		exit(0);
  	}
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sockfd,(struct sockaddr*)&addr ,sizeof(addr));

  // marks the socket referred to by sockfd as a passive socket,
  // that is, as a socket that will be used to accept incoming connection
  // requests using accept(2).
  listen (sockfd, 5);

  for (int i=0;i<5;i++)
  {
    memset(&client, 0, sizeof (client));
    addrlen = sizeof(client);
    pollfds[i].fd = accept(sockfd,(struct sockaddr*)&client, (socklen_t*)&addrlen);
    pollfds[i].events = POLLIN;
  }
  sleep(1);
  while(1){
  	puts("round again");
	poll(pollfds, 5, 50000);

	for(i=0;i<5;i++) {
		if (pollfds[i].revents & POLLIN){
			pollfds[i].revents = 0;
			memset(buffer,0,MAXBUF);
			read(pollfds[i].fd, buffer, MAXBUF);
			puts(buffer);
		}
	}
  }

}
