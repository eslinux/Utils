// THIS IS A DEPRECATED METHOD OF GETTING HOST NAMES
// use getaddrinfo() instead!

#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int i;
	struct hostent *he;
	struct in_addr **addr_list;

	char myname[256];
	gethostname(myname, 255);

	if ((he = gethostbyname(myname)) == NULL) {  // get the host info
		herror("gethostbyname");
		return 2;
	}

	// print information about this host:
	printf("Official name is: %s\n", he->h_name);
	printf("    IP addresses: ");
	addr_list = (struct in_addr **)he->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++) {
		printf("%s ", inet_ntoa(*addr_list[i]));
	}
	printf("\n");

	return 0;
}
