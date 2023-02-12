#include <stdlib.h>
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <execinfo.h>
#include <sys/errno.h>

/**
 * Buffer sizes
 * Sizes of strings
 * Static memory is faster than dynamic
 * However dynamic memory can be more precise
 */
#define SMALL 64
#define MEDIUM SMALL * 4
#define LARGE MEDIUM * 4
#define XL LARGE * 2

// Linked list of endpoints
struct Endpoint {
	char *path;
};

/**
 * baseURL is a FQDN
 * endpoint is an infinite list of endpoints
 */
struct URI {
	char *baseUrl;
	char **endpoint;
};

/**
 * An object returned from every function
 */
struct Return {
};

/**
 * Line number
 * Stack
 * Message
 */
struct Error {
	char message[MEDIUM];
	int err_num;
};



/**
 * Defined in netdb.h
 * man getaddrinfo
 * reimplementation
 * no addrinfo man page
 */
struct addyinfo {
	int flags; // input flags
	int family; //protocol family for socket
	int protocol; //protocol for socket
	socklen_t addylen; //length of socket-address
	struct sockaddr *addy; //socket address for socket
	struct addyinfo *next; //pointer to next in list
};

/**
 * Actual implementation
 * 	int ai_flags; // input flags
 *	int ai_family; //protocol family for socket
 *	int ai_socktype; //socket type 
 *	int ai_protocol; //protocol for socket
 *	int socklen_t ai_addrlen; //length of socket-address
 *	struct sockaddr *ai_addr; //socket address for socket
 *	char *ai_canonname; // cannonical name for service location
 *	struct addrinfo *ai_next; //pointer to next in list
 */

// Function declarations
void print_sockets(struct addrinfo *addy_info);
void print_socket(struct addrinfo *addy_info);
char* sockaddr_tostring(struct sockaddr *sockaddy);

// Helper functions
void print_callstack();
void handle_snprintf(int ret, size_t size);
void fail();
void print_ip(struct addrinfo *addr);

int main() {
	int fd;
	struct addyinfo addy_info;

	/**
	 * Beej Guide
	 * "anyone who says TCP never arrives out of order I wont listen lalala"
	 * AF = address family
	 * memset(&hints, 0, sizeof(addrinfo));
	 * hints.ai_family = AF_INET6; 
	 * hints.ai_socktype = SOCK_DGRAM;
	 * hints.ai_flags = AI_PASSIVE;
	 */


	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = 0;
	hints.ai_flags = AI_PASSIVE;


	/**
	 * man page
	 * getaddrinfo gets a list of IP addresses and port names for hostname and servname
	 * 	- hostname & servname are null pointer or null terminated strings
	 * 		  TS
	 * 		  Neither can be null
	 * 		  One must be string
	 * 		  Both can be string
	 * 		  Neither can't be strings
	 * 	- hostname is valid host name or string IPv4/6 addresss
	 * 	- servname is port number of service name listed in /etc/services services(5)
	 TS
	 char *host = "addy.address"
	 Map to /etc/hosts localhost
	 Now go to addy.address in chrome browser
	 Useful if TLS cert != localhost and running locally
	 * replaces gethostbyname and getservbyname()
	 */
	char *host = "127.0.0.1"; //"localhost";
	struct addrinfo *potential_hosts;
	char *port = "443";
	int err_num = 0; 
	struct Error error;
	memset(&error, 0, sizeof(struct Error));
	if ((err_num = getaddrinfo("localhost", "8000", &hints, &potential_hosts)) != 0) {
		struct Error error = error;
		error.err_num = err_num;
		fail(error);
		return -1;
	}

	print_sockets(potential_hosts);


	/*
	 * Function for actually setting the functions
	 fd = make_socket(potential_hosts);
	 if (!fd) return -1;
	 */




	return 0;
}

/*
 * Print out the contents of the sockaddr structure
 * String is allocated on the heap dynamically
 * So the string returned should be freed after usage is finished
 * I think it's in sys/socket.h
 * somehow sockaddr can be cast to sockaddr_in or sockaddr_in6
 * using sockaddr->data was resulting in odd behavior 
 */
char* sockaddr_tostring(struct sockaddr *sockaddy) {
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

	char *buffer = malloc(sizeof(char) * MEDIUM);
	int ret = snprintf(buffer, sizeof(char) * MEDIUM,
			   "struct sockaddr {\n\
			   \tfamily = %i %s\n\
			   \taddress = %s\n\
			   \tport = %i\n\
			   }\n",
			   sockaddy->sa_family, family,
			   host_str,
			   htons(port));
	handle_snprintf(ret, sizeof(char) * MEDIUM);

	return buffer;
}

/**
 * Prints multiple sockets
 */
void print_sockets(struct addrinfo *addy_info) {
	while(addy_info) {
		print_socket(addy_info);
		print_ip(addy_info);
		addy_info = addy_info->ai_next;
	}
}

/**
 * Prints out useful information about the socket
 */
void print_socket(struct addrinfo *addy_info) {
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


	char *sockaddr_str = sockaddr_tostring(addy_info->ai_addr);

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
 * TS
 * Gracefully exit the program
 * Accessible from anywhere
 * throw stack
 */
void fail(struct Error error) {
	if (error.err_num != 0) {
		const char *msg = gai_strerror(error.err_num); 
		fprintf(stderr, "getaddrinfo: %s\n", msg);
		print_callstack();
	}
};


/**
 * Beej
 * Print IP's returned from getaddrinfo
 */
void print_ip(struct addrinfo *p) {
	void *addr;
	char *ipver;
	char ipstr[INET6_ADDRSTRLEN];

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

/*
   int fd = make_socket(res) {
// Try with hints being null
while (res) {
print_socket(res);
res = res->ai_next;
}
}
*/
