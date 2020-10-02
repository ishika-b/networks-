#include<iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>



using namespace std;

#define report(message, retcode) {printf(message); return retcode;}




int main() {
 cout<<"hello world!"<<endl;
 int fd; 

 if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("cannot create socket");
	return 1;
 }

 struct in_addr inp;
 if(inet_aton("0.0.0.0", &inp) != 1){
 	report("iton",1);
 }

 struct sockaddr_in saddr;
 memset(&saddr, 0, sizeof(saddr));
 saddr.sin_family = AF_INET;
 saddr.sin_addr = inp;
 saddr.sin_port = htons(8000);
 if(bind(fd,(struct sockaddr*) &saddr, sizeof(saddr))){
 	report("bind",1);
 }

 char server_says[] = "server\r\n";
 ssize_t done;
 char buf[1500] = {0};
 struct sockaddr src_addr;
 socklen_t adr_len =sizeof(src_addr);
 ssize_t got;
 while(1){
 	got = recvfrom(fd, buf, sizeof(buf),0, &src_addr, &adr_len);
 	buf[got]=0;
 	printf("%s",buf);
    done = sendto(fd, server_says, strlen(server_says), 0, &src_addr, adr_len);
}

 return 0;
}