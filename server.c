#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

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
 * TS
 * Gracefully exit the program
 * Accessible from anywhere
 * throw stack
 */
void fail(struct Error error) {
	if (error.err_num != 0) {
		const char *msg = gai_strerror(error.err_num); 
		fprintf(stderr, "getaddrinfo: %s\n", msg);
	}
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


int main() {
	int fd;
	struct addyinfo addy_info;

	/**
	  * Beej Guide
	  * "anyone who says TCP never arrives out of order I wont listen lalala"
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
	 * 	- servname is port number of service name listed in services(5)
	 		TS
	 		char *host = "addy.address"
			Map to /etc/hosts localhost
			Now go to addy.address in chrome browser
			Useful if TLS cert != localhost and running locally
	 * replaces gethostbyname and getservbyname()
	 */
	char *host = "localhost";
	struct addrinfo *potential_hosts;
	char *port = "443";
	int err_num = 0; 
	struct Error error;
	memset(&error, 0, sizeof(struct Error));
	if ((err_num = getaddrinfo(host, port, &hints, &potential_hosts)) != 0) {
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
 * I think it's in sys/socket.h
 */
char *sockaddr_tostring(struct sockaddr *sockaddy) {
	char *family;
	if (sockaddy->sa_family == AF_INET) 
		family = "IPv4";
	else if (sockaddy->sa_family == AF_INET6)
		family = "IPv6";
	else 
		family = "Unknown family";
		
	char buffer[MEDIUM];
	snprintf(buffer, sizeof(char) * MEDIUM,
		 "struct sockaddr {\n\
	         \tfamily = %i %s\n\
	         \tdata = %s\n\
	         }\n",
	       sockaddy->sa_family, family,
	       sockaddy->sa_data);
	char *ptr = buffer;
	return ptr;
}

/**
 * Prints multiple sockets
 */
void print_sockets(struct addrinfo *addy_info) {
	while(addy_info) {
		print_socket(addy_info);
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
	       sockaddr_tostring(addy_info->ai_addr),
	       addy_info->ai_canonname);

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
