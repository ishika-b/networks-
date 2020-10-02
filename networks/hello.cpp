#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <vector>


using namespace std;

#define report(message, retcode) {printf(message); return retcode;}

struct dns_header {
  uint16_t id;
  uint16_t flags;
  uint16_t num_q;
  uint16_t num_ansrr = 0;
  uint16_t num_autrr = 0;
  uint16_t num_addrr;
};

vector<unsigned char> vectorofDN(const string& s){
 vector<unsigned char> dn;
 dn.push_back(0);
 int i = 0;
 for(const char ch : s){
  if(ch != '.'){
    dn[i]++;
    dn.push_back(ch);
  }
  if(ch == '.'){
    i = dn.size();
    dn.push_back(0);
  }
 }
 dn.push_back(0);
 return dn;
}


int main(){
 cout<<"hello world!"<<endl;

 int fd; 

 if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("cannot create socket");
	return 1;
 }


  /* get the address of the host */
  //const string& address = "8.8.8.8";
  struct hostent* hptr = gethostbyname("192.168.1.1"); /* localhost: 127.0.0.1 */
  if (!hptr) report("gethostbyaddr", 1); /* is hptr NULL? */
  if (hptr->h_addrtype != AF_INET)       /* versus AF_LOCAL */
    report("bad address family", 1);

  /* connect to the server: configure server's address 1st */
  struct sockaddr_in saddr;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr =
     ((struct in_addr*) hptr->h_addr_list[0])->s_addr;
  saddr.sin_port = htons(53); /* port number in big-endian */
  if (connect(fd, (struct sockaddr*) &saddr, sizeof(saddr)) < 0){
    cout<<errno<<endl;
    report("connect", 1);
  }

 struct dns_header hdr;
 hdr.id = htons(1);
 hdr.flags = htons(0b0000000100000000);
 hdr.num_q = htons(1);
 hdr.num_addrr = htons(1);
 vector<unsigned char> dn = vectorofDN("varstack.com");

 uint16_t query_type = htons(1);
 uint16_t query_class = htons(1);
 
 const string& flush = "\r\n\r\n";
 const char add_records[] = "\x00\x00\x29\x02\x00\x00\x00\x00\x00\x00\x00";

 uint8_t pkt[sizeof(hdr)+dn.size()+4+11+flush.size()];
 uint8_t* packet = pkt;
 memcpy(packet,&hdr,sizeof(hdr));
 packet = packet + sizeof(hdr);
 memcpy(packet,dn.data(),dn.size());
 packet = packet + dn.size();
 memcpy(packet,&query_type,2);
 packet = packet + 2;
 memcpy(packet,&query_class,2);
 packet = packet + 2;
 cout<<(sizeof(add_records))<<endl;
 memcpy(packet,add_records,11);
 packet = packet + 11;
 memcpy(packet,flush.data(),flush.size());

 ssize_t done = write(fd,pkt,sizeof(pkt));
 cout<<done<<endl;

 unsigned char buf[1500] = {0};
 ssize_t got;
 got = read(fd,buf,1000);
  for(int i = 0; i < got; i++){
	printf("%.2x ",buf[i]);
  }

 return 0;
}