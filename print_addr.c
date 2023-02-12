/*
 ** showip.c -- show IP addresses for a host given on the command line
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>

#define MEDIUM 128

char* sockaddr_tostring(struct sockaddr *sockaddy);

int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	if (argc != 2) {
		fprintf(stderr,"usage: hostname\n");
		return 1;
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}


	printf("IP addresses for %s:\n\n", argv[1]);

	for(p = res;p != NULL; p = p->ai_next) {
		char *addy = sockaddr_tostring(res->ai_addr);
		printf("tommy code says %s\n", addy);
		free(addy);

		void *addr;
		char *ipver;

		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		// convert the IP to a string and print it:
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		printf("  %s: %s\n", ipver, ipstr);
	}

	freeaddrinfo(res); // free the linked list

	return 0;
}

char* sockaddr_tostring(struct sockaddr *sockaddy) {
	char *family;
	if (sockaddy->sa_family == AF_INET) {
		family = "IPv4";
	}
	else if (sockaddy->sa_family == AF_INET6) {
		family = "IPv6";
	}
	else { 
		family = "Unknown family";
	}

	/**
	 * inet_ntop: Convert binary numbers to IP address
	 * 	ntop: network to printable or network text presentation
	 */
	char host[MEDIUM];
	if(inet_ntop(sockaddy->sa_family,
		     sockaddy,
		     host,
		     sizeof(char) * MEDIUM) == NULL) {
		int err =  errno;
		fprintf(stderr, "inet_ntop failure %s", strerror(err));
	}

	char *buffer = malloc(sizeof(char) * MEDIUM);
	int ret = snprintf(buffer, sizeof(char) * MEDIUM,
			   "struct sockaddr {\n\
			   \tfamily = %i %s\n\
			   \taddress = %s\n\
			   }\n",
			   sockaddy->sa_family, family,
			   host);

	return buffer;
}


