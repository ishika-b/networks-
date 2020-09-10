#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>


using namespace std;

#define report(message, retcode) {printf(message); return retcode;}



int main(){
 cout<<"hello world!"<<endl;

 int fd; 

 if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("cannot create socket");
	return 1;
 }


  /* get the address of the host */
  struct hostent* hptr = gethostbyname("localhost"); /* localhost: 127.0.0.1 */
  if (!hptr) report("gethostbyname", 1); /* is hptr NULL? */
  if (hptr->h_addrtype != AF_INET)       /* versus AF_LOCAL */
    report("bad address family", 1);

  /* connect to the server: configure server's address 1st */
  struct sockaddr_in saddr;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr =
     ((struct in_addr*) hptr->h_addr_list[0])->s_addr;
  saddr.sin_port = htons(8000); /* port number in big-endian */
  if (connect(fd, (struct sockaddr*) &saddr, sizeof(saddr)) < 0)
    report("connect", 1);
 
 char get[] = "GET / HTTP/1.1\r\n\r\n";
 ssize_t done = write(fd,get,strlen(get));

 cout<<done<<endl;
 
 char buf[1500] = {0};
 ssize_t got;
 while(got = read(fd,buf,1000)){
 	buf[got]=0;
	printf("%s",buf);
 }

 return 0;
}