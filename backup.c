#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
struct Endpoint {
	char *path;
}

/**
 * baseURL is a FQDN
 * endpoint is an infinite list of endpoints
 */
struct URI {
	char *baseUrl;
	char **endpoint;
}

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
	char *message;
}

/**
 * TS
 * Gracefully exit the program
 * Accessible from anywhere
 */
struct Error fail(char *msg) {
	// throw stack
	Error error;
	const char *msg = gai_strerror(err); 
	err.message = msg; 
	fprintf(stderr, "getaddrinfo: %s\n", msg);
	return error;
};
		

/**
  * TS
  *next* 
  &*-> address location of type addyinfo

  ***addy***
  URInary track infection
  host
  base_url
  endpoints

  ***addylen***
  ###### of characters

  ***protocol***
  HTTPS
  HTTP
  FTP
  TCP
  UDP
  TLS 
  	SYN->SYN ACK->ACK(hi)->cert->key
	Developed for HTTP
	Replaced SSL
	Specify TLS version
	  * default to highest one available
	Decide on cipher suites
	server public key and CA 
	Generate session keys after handshake is complete
  SSL
  fenny


  ***FAMILY***
  brother
  sister
  parents

  ***FLAGS***
  red
  green
  blue
*/

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
	int socklen_t addylen; //length of socket-address
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

int main() {
	int fd;
	struct addrinfo hints;
	struct addyinfo addy_info;

	/**
	  * Beej Guide
	  * "anyone who says TCP never arrives out of order I wont listen lalala"
	  * memset(&hints, 0, sizeof(addrinfo));
	  * hints.ai_family = AF_INET6; 
	  * hints.ai_socktype = SOCK_DGRAM;
	  * hints.ai_flags = AI_PASSIVE;
	  */

	/**
	  * man getaddrinfo 
	  * @var ai_family: Only accept protocols supported by the os
	  * @var ai_socktype: Stream or datagram (ssh && HTTP use stream sockets)
	   	- SOCK_STREAM uses TCP: Transmission Control Protocol
		- SOCK_DGRAM uses UDP: may arrive out of order
			- Just ignore em or approximate it
	  * @var flags: first 10 bits matter
	  	- Which is first?
		- If you aint first you last
			- if you aint FIFO you LIFO
		- Should fit in 8 bits
		AI_ADDRCONFIG System decides IVP4 or IPV6
		AI_ALL 
		AI_CANNONAME
		AI_NUMERICHOST hostname is IPv4 or IP6 or DNS should be performed
		AI_NUMERICSERV
		AI_PASSIVE 
			1. TCP: returned socket address structure will use bind
				- hostname will be null pointer
				- IP address will be set 
					- INADDR_ANY for IPv4
					- IN6ADDR_ANY_INIT for IPv6
			0. UDP: returned socket address struct will use connect, sendto, sendmsg
				- if hostname is null pointer && AI_PASSIVE === 0
					- IP address will be set to loopback addresss 

		AI_V4MAPPED
		AI_V4MAPPED_CFG
		AI_DEFAULT
		AI_UNUSABLE	
	  */
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = 0;
	hints.ai_flags = AI_PASSIVE;


	/*
	 * TS
	char *host = "addy.address"
	Map to /etc/hosts localhost
	Now go to addy.address in chrome browser
	Useful if TLS cert != localhost and running locally
	*/
	char *host = "localhost";
	// Linked list of endpoints
	char *port = "443";
	struct addrinfo *server;
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
	 * replaces gethostbyname and getservbyname()
	 */
	if (!getaddrinfo(host, service, &hints, &res)) {
		Error err = fail();
		return -1;
	}

	fd = make_socket(hints);
	if (!fd) return -1;
	



	return 0;
}

/**
 * Prints out useful information about the socket
 */
void print_socket(struct addrinfo addy_info) {
	char message[256];
	char *family, *socktype, *protocol, *flags;
	if (addy_info.ai_family == PF_UNSPEC)
		family = "
		
	snprintf(msg, "Will accept
}

int fd = make_socket(res) {
	// Try with hints being null
	while (res) {
		res = res->ai_next;
	}
}
