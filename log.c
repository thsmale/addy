#include "addy.h"
/**
 * Prints out useful information about the socket
 * @param addy_info is a single struct or linked list of addrinfo
 * 	- addrinfo structs are incremented with the ai_next property
 */
void print_host(struct addrinfo *addy_info) {
	while (addy_info != NULL) {
		char *family, *socktype, *protocol;
		if (addy_info->ai_family == PF_UNSPEC)
			family = "Will accept any protocol supported by OS";
		else if (addy_info->ai_family == AF_INET)
			family = "IPv4 addresses only";
		else if (addy_info->ai_family == AF_INET6)
			family = "IPv6 addresses only";
		else
			family = "Unknown family";

		if (addy_info->ai_socktype == 0)
			socktype = "Any socket type is accepted";
		else if (addy_info->ai_socktype == SOCK_STREAM) 
			socktype = "TCP stream";
		else if (addy_info->ai_socktype == SOCK_DGRAM)
			socktype = "UDP datagram";
		else if (addy_info->ai_socktype == SOCK_RAW)
			socktype = "Raw socket";
		else 
			socktype = "Unknown socket type" ;

		if (addy_info->ai_protocol == 0)
			protocol = "Will accept any protocol";
		if (addy_info->ai_protocol == IPPROTO_UDP)
			protocol = "UDP protocol";
		else if (addy_info->ai_protocol == IPPROTO_TCP)
			protocol = "TCP protocol";
		else 
			protocol = "Unknown protocol";


		char host_name[MEDIUM];
		sockaddr_tostring(addy_info->ai_addr, host_name);

		printf("struct addrinfo {\n\
		       \tflags = %i\n\
		       \tprotocol family for socket = %i %s\n\
		       \tsocket type = %i %s\n\
		       \tprotocol for socket= %i %s\n\
		       \tlength of socket-address = %i\n\
		       \tsocket address = %s\n\
		       \tcanonical name for service location = %s\n}\n", 
		       addy_info->ai_flags,
		       addy_info->ai_family, family,
		       addy_info->ai_socktype, socktype,
		       addy_info->ai_protocol, protocol,
		       addy_info->ai_addrlen, 
		       sockaddr_str,
		       addy_info->ai_canonname);

		free(sockaddr_str);
		addy_info = addy_info->ai_next;
	}
}


