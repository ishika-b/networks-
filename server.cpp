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

 if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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
 
 if(listen(fd,100)){
 	report("listen",1);
 }

 int fd_listen = accept(fd,NULL,NULL);
 if(fd_listen < 0 ){
 	report("accept failed",1);
 }

 
 char buf[1500] = {0};
 ssize_t got;
 while(1){
 	got = read(fd_listen,buf,1000);
 	buf[got]=0;
	printf("%s",buf);
 }

 return 0;
}