//https://jvns.ca/blog/2017/06/03/async-io-on-linux--select--poll--and-epoll/
//node.js on Linux, it’s actually using the epoll Linux system call under the hood.
//select & poll
//These 2 system calls are available on any Unix system, while epoll is Linux-specific. Here’s basically how they work:


/*
Epoll vs Select/Poll
We can add and remove file descriptor while waiting
epoll_wait returns only the objects with ready file descriptors
epoll has better performance – O(1) instead of O(n)
epoll can behave as level triggered or edge triggered (see man page)
epoll is Linux specific so non portable
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
#include <sys/epoll.h>
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
int epoll_main()
{
  char buffer[MAXBUF];
  struct sockaddr_in addr;
  struct sockaddr_in client;
  int addrlen, i,nfds;

  struct epoll_event events[5];
  int epfd = epoll_create(10);
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
  listen (sockfd, 5);

  for (i=0;i<5;i++)
  {
    static struct epoll_event ev;
    memset(&client, 0, sizeof (client));
    addrlen = sizeof(client);
    ev.data.fd = accept(sockfd,(struct sockaddr*)&client, (socklen_t*)&addrlen);
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
  }

  sleep(1);
  while(1){
  	puts("round again");
  	nfds = epoll_wait(epfd, events, 5, 10000);

	for(i=0;i<nfds;i++) {

			memset(buffer,0,MAXBUF);
			read(events[i].data.fd, buffer, MAXBUF);
			puts(buffer);
	}
  }

}

/*strace
execve("Debug/epoll_ex", ["Debug/epoll_ex", "1"], [/ 72 vars /]) = 0
brk(NULL)                               = 0x842000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=149150, ...}) = 0
mmap(NULL, 149150, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f50e162e000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
open("/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\t\2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=1868984, ...}) = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f50e162d000
mmap(NULL, 3971488, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f50e1064000
mprotect(0x7f50e1224000, 2097152, PROT_NONE) = 0
mmap(0x7f50e1424000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1c0000) = 0x7f50e1424000
mmap(0x7f50e142a000, 14752, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f50e142a000
close(3)                                = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f50e162c000
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f50e162b000
arch_prctl(ARCH_SET_FS, 0x7f50e162c700) = 0
mprotect(0x7f50e1424000, 16384, PROT_READ) = 0
mprotect(0x601000, 4096, PROT_READ)     = 0
mprotect(0x7f50e1653000, 4096, PROT_READ) = 0
munmap(0x7f50e162e000, 149150)          = 0
epoll_create(10)                        = 3
clone(child_stack=0, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f50e162c9d0) = 13996
clone(child_stack=0, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f50e162c9d0) = 13997
clone(child_stack=0, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f50e162c9d0) = 13998
clone(child_stack=0, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f50e162c9d0) = 13999
clone(child_stack=0, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f50e162c9d0) = 14000
socket(PF_INET, SOCK_STREAM, IPPROTO_IP) = 4
bind(4, {sa_family=AF_INET, sin_port=htons(2000), sin_addr=inet_addr("0.0.0.0")}, 16) = 0
listen(4, 5)                            = 0
accept(4, {sa_family=AF_INET, sin_port=htons(53818), sin_addr=inet_addr("127.0.0.1")}, [16]) = 5
epoll_ctl(3, EPOLL_CTL_ADD, 5, {EPOLLIN, {u32=5, u64=5}}) = 0
accept(4, {sa_family=AF_INET, sin_port=htons(53820), sin_addr=inet_addr("127.0.0.1")}, [16]) = 6
epoll_ctl(3, EPOLL_CTL_ADD, 6, {EPOLLIN, {u32=6, u64=6}}) = 0
accept(4, {sa_family=AF_INET, sin_port=htons(53822), sin_addr=inet_addr("127.0.0.1")}, [16]) = 7
epoll_ctl(3, EPOLL_CTL_ADD, 7, {EPOLLIN, {u32=7, u64=7}}) = 0
accept(4, {sa_family=AF_INET, sin_port=htons(53826), sin_addr=inet_addr("127.0.0.1")}, [16]) = 8
epoll_ctl(3, EPOLL_CTL_ADD, 8, {EPOLLIN, {u32=8, u64=8}}) = 0
accept(4, {sa_family=AF_INET, sin_port=htons(53828), sin_addr=inet_addr("127.0.0.1")}, [16]) = 9
epoll_ctl(3, EPOLL_CTL_ADD, 9, {EPOLLIN, {u32=9, u64=9}}) = 0
nanosleep({1, 0}, 0x7ffe63b78830)       = 0
fstat(1, {st_mode=S_IFREG|0664, st_size=3332, ...}) = 0
brk(NULL)                               = 0x842000
brk(0x864000)                           = 0x864000
epoll_wait(3, [{EPOLLIN, {u32=5, u64=5}}], 5, 10000) = 1
read(5, "Test message 2 from client 13996", 256) = 32
epoll_wait(3, [{EPOLLIN, {u32=7, u64=7}}, {EPOLLIN, {u32=8, u64=8}}], 5, 10000) = 2
read(7, "Test message 2 from client 13997", 256) = 32
read(8, "Test message 2 from client 13999", 256) = 32
epoll_wait(3, [{EPOLLIN, {u32=9, u64=9}}], 5, 10000) = 1
read(9, "Test message 2 from client 14000", 256) = 32
epoll_wait(3, strace: Process 13995 detached
 <detached ...>*/
