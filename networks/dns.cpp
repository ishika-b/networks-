
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

// struct sockaddr_in myaddr;

// //af_inet is IP, sock_dgram for UDP, only one protocol so 0
// if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
// 	perror("cannot create socket");
// 	return 0;
// }

// myaddr.sin_family = AF_INET;
// myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
// myaddr.sin_port = htons(53);

// if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
// 	perror("bind failed");
// 	return 0;
// }

//dns_header_t header;

#include <stdlib.h>
#include <netdb.h>

/* paddr: print the IP address in a standard decimal dotted format */
void
paddr(unsigned char *a)
{
        printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
}

int main(int argc, char **argv) {
	struct hostent *hp;
	char host[] = "google.com";
	int i;

	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "could not obtain address of %s\n", host);
		return 0;
	}
	for (i=0; hp->h_addr_list[i] != 0; i++)
		paddr((unsigned char*) hp->h_addr_list[i]);
	exit(0);
}


