#include "addy.h"

/*
 * Print out the contents of the sockaddr structure
 * String is allocated on the heap dynamically
 * So the string returned should be freed after usage is finished
 * I think it's in sys/socket.h
 * somehow sockaddr can be cast to sockaddr_in or sockaddr_in6
 * using sockaddr->data was resulting in odd behavior 
 * @param sockaddr sockaddy contains the host address and port
 * @param string buffer will equal the stringified sockaddr 
 * @returns also returns the buffer
 */
char* sockaddr_tostring(struct sockaddr *sockaddy, char *buffer) {
	char *family;
	int port;
	void *host;
	if (sockaddy->sa_family == AF_INET) {
		family = "IPv4";
		struct sockaddr_in *sockaddy_in = (struct sockaddr_in *)sockaddy;
		host = &(sockaddy_in->sin_addr);
		port = sockaddy_in->sin_port;
	}
	else if (sockaddy->sa_family == AF_INET6) {
		family = "IPv6";
		struct sockaddr_in6 *sockaddy_in = (struct sockaddr_in6 *)sockaddy;
		host = &(sockaddy_in->sin6_addr);
		port = sockaddy_in->sin6_port;
	}
	else { 
		return "Unknown family";
	}

	/**
	 * inet_ntop: Convert binary numbers to IP address
	 * 	ntop: network to printable or network text presentation
	 */
	char host_str[INET6_ADDRSTRLEN];
	if(inet_ntop(sockaddy->sa_family,
		     host,
		     host_str,
		     sizeof(host_str)) == NULL) {
		int err =  errno;
		fprintf(stderr, "inet_ntop failure %s", strerror(err));
		print_callstack();
	}

	int ret = snprintf(buffer, sizeof(char) * MEDIUM,
			   "struct sockaddr {\n\
			   \tfamily = %i %s\n\
			   \taddress = %s\n\
			   \tport = %i\n\
			   }",
			   sockaddy->sa_family, family,
			   host_str,
			   htons(port));
	handle_snprintf(ret, sizeof(char) * MEDIUM);

	return buffer;
}

/**
 * Form a socket that we can listen to connections on
 * @param potential_host the link listed returned from getaddrinfo
 * @returns -1 when fails  or a file descriptor to listen on
 */
int set_socket(struct addrinfo *potential_host) {
	int fd = -1;
	while (potential_host) {
		/**
		 * socket creates an endpoint for communication
		 * @param ai_family: IPv4 or IPv6
		 * @param socktype: SOCK_STREAM, SOCK_DGRAM, or SOCK_RAW
		 * @param protocol: TCP or UDP
		 * 	- can have one port that supports multiple protocols
		 * @returns file descriptor 
		 */
		fd = socket(potential_host->ai_family,
			    potential_host->ai_socktype,
			    potential_host->ai_protocol);
		if(fd == -1) {
			perror("socket: \n");
			potential_host = potential_host->ai_next;
			continue;
		}
		/**
		 * Associate socket with a specific port
		 * So we can listen to incomming connections
		 * @param socket so socket can be assigned to the address
		 * @param sockaddr contains host, port, IPv4 or IPv6
		 * @param socklen_t len of address
		 * @returns 0 in success and -1 for failure and sets errno
		 */
		if(bind(fd, potential_host->ai_addr, potential_host->ai_addrlen) == -1) {
			perror("socket: \n");
			potential_host = potential_host->ai_next;
			continue;
		}
		// Successfully made a socket
		char buffer[MEDIUM];
		char *server_info = sockaddr_tostring(potential_host->ai_addr, buffer);
		printf("Socket %i created for server %s\n", fd, server_info);
		free(server_info);
		break;
	}
	free(potential_host);
	return fd;
}

/**
 * Print the current callstack
 * Useful for tracing back steps when debugging
 * man backtrace
 * execinfo.h file
 */
void print_callstack() {
	void *buf[LARGE];
	/**
	 * @param buf will write addresses of current callstack as array of pointers to this value
	 * @param size set MAX number of pointers to write
	 * @return number of pointers actually written to array
	 */
	int stack_size = backtrace(buf, sizeof(char) * LARGE);
	/**
	 * convert call stack from backtrace into human readable strings using dladdr
	 * @param buf pointers containing addresses of current callstack
	 * @param size determins size of array of strings that is returned
	 * @returns array of human readable strings describing the stack
	 */
	char **stack = backtrace_symbols(buf, stack_size);
	for (int i = 0; i < stack_size; ++i) {
		printf("%s\n", stack[i]);
	}
	/**
	 * backtrace_symbols allocates memory for array of human readable strings using malloc
	 * so up to us to release the memory when finished
	 */
	free(stack);
}


/**
 * TODO: integrate with Fail
 * Output from snprintf is always null terminated
 * @param ret return value from snprintf
 * @param size size argument passed to snprintf
 */
void handle_snprintf(int ret, size_t size) {
	char *msg = "\0";
	if (ret >= size) 
		msg = "str too small, some characters were discarded";
	else if (size == 0)
		msg = "str was not null terminated since size is zero";
	if (strcmp(msg, "\0") != 0) {
		fprintf(stderr, "snprintf error %s\n", msg);
		print_callstack();
	}

}


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
		       host_name,
		       addy_info->ai_canonname);

		addy_info = addy_info->ai_next;
	}
}


