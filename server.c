#include <stdlib.h>
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <execinfo.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>

/**
 * TODO: make these more dynamic
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
	char *message;
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
void print_ip(struct addrinfo *addr);
int make_socket(struct addrinfo *potential_host);

void print_callstack();
void handle_snprintf(int ret, size_t size);
void fail();

int main() {
	struct addyinfo addy_info;

	/**
	 * Specify what kind of sockets we want
	 * @var ai_family: IPv4 or IPv6 but let operating system pick
	 * @var ai_socktype: TCP or UDP
	 * @var ai_flags: returned socket will use bind
	 */
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	/**
	 * getaddrinfo returns a linked list of IP address and port names based on hostname and port
	 */
	struct addrinfo *potential_hosts;
	char *host = "localhost"; // or "127.0.0.1"
	char *port = "443";
	int err_num = 0; 
	if ((err_num = getaddrinfo("localhost", "8000", &hints, &potential_hosts)) != 0) {
		struct Error error;
		error.err_num = err_num;
		fail(error);
		return -1;
	}

	print_sockets(potential_hosts);

	int fd = make_socket(potential_hosts);
	if (fd == -1) {
		struct Error error;
		error.message = "Failed to create socket";
		fail(error);
		return -1;
	}

	/**
	 * listen for incomming connections and set queue limit
	 * @param socket: created from socket(2) 
	 * @param backlog: max length of queue for pending connections
	 * 	- ECONNREFUSED will be returned if request arrives when queue is full
	 * 	- Note the max value of this is 128 as per BUGS section (man listen)
	 * @returns 0 for success, -1 and sets errno for failure
	 */
	int max_connections = 100;
	if (listen(fd, max_connections) == -1) {
		perror("listen: \n");
		close(fd);
		return -1;
	}

	/**
	 * Extracts first connection request on the queue of pending connections
	 * @param socket a socket that has been created with socket(2), bind(2), and listen(2)
	 * @param sockaddr pointer to sockaddr this is filled in with connection details of client
	 * @param addr_len pointer to socklen_t that will be set to length in bytes of address 
	 * 	- should initially contain amount of space pointed to by param sockaddr
	 * @returns fd of new socket containing this connection or -1 on failure and sets errno
	 * 	- this returned socket contains the same properties of the socket passed in
	 */
	struct sockaddr new_connection;
	socklen_t address_len = sizeof(new_connection);
	int new_fd; 
	while (1) {
		if((new_fd = accept(fd, &new_connection, &address_len) == -1)) {
			perror("accept: \n");
			continue;
		}
		printf("Accepted new conection from %s\n", sockaddr_tostring(&new_connection));
		if(!fork()) {
			/**
			 * @param socket the file descriptor receiving the data
			 * @param buffer
			 * @param length
			 * @param flags
			 * @return number of bytes received
			 * 	- 0: if no messages are able to be recieved and peer has performed shutdown
			 * 	- -1: errno is set
			 */
			char buffer[LARGE];
			int ret = recv(new_fd, buffer, sizeof(char)*LARGE-1, 0);
			buffer[ret] = '\0';
			printf("Received %s from client\n", buffer);
			if (ret == 0) {
				printf("Should close connection now\n");
				exit(1);
			} else if (ret < 0) {
				printf("recv error: \n");
				exit(-1);
			}
		}
		close(new_fd);
	}

	close(fd);
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
			   }",
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

/**
 * Form a socket that we can listen to connections on
 * @param potential_host the link listed returned from getaddrinfo
 * @returns -1 when fails  or a file descriptor to listen on
 */
int make_socket(struct addrinfo *potential_host) {
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
		char *server_info = sockaddr_tostring(potential_host->ai_addr);
		printf("Socket %i created for server %s\n", fd, server_info);
		free(server_info);
		break;
	}
	free(potential_host);
	return fd;
}

