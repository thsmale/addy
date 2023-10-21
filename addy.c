#include "addy.h"

/**
 * Start a server that will listen to connections on a socket
 * By default this is a SOCK_STREAM server to be used with TCP/HTTP
 * @param host a FQDN or IP address
 * @param port a port to bind to, port should respect /etc/services file
 * @return the file descriptor to read and write streams to
 */
int start_server(char *host, char *port) {
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
	int err_num = 0; 
	if ((err_num = getaddrinfo(host, port, &hints, &potential_hosts)) != 0) {
		fprintf(stderr, "getaddrinfo failed because %s\n", gai_strerror(err_num));
		return -1;
	}

	int fd = -1;
	char host_info[MEDIUM]; //stringified version of struct addrinfo
	while (potential_hosts) {
		sockaddr_tostring(potential_hosts->ai_addr, host_info);
		/**
		 * socket creates an endpoint for communication
		 * @param ai_family: IPv4 or IPv6
		 * @param socktype: SOCK_STREAM, SOCK_DGRAM, or SOCK_RAW
		 * @param protocol: TCP or UDP
		 * 	- can have one port that supports multiple protocols
		 * @returns file descriptor 
		 */
		fd = socket(potential_hosts->ai_family,
			    potential_hosts->ai_socktype,
			    potential_hosts->ai_protocol);
		if(fd == -1) {
			printf("unable to make socket %i for %s because %s\n", fd, host_info, strerror(errno));
			perror("socket error");
			potential_hosts = potential_hosts->ai_next;
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
		if(bind(fd, potential_hosts->ai_addr, potential_hosts->ai_addrlen) == -1) {
			printf("unable to bind socket %i to %s because %s\n", fd, host_info, strerror(errno));
			perror("bind error");
			potential_hosts = potential_hosts->ai_next;
			close(fd);
			continue;
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
			printf("unable to listen on socket %i because %s", fd, strerror(errno));
			perror("listen error");
			potential_hosts = potential_hosts->ai_next;
			close(fd);
			continue;
		}

		// Successfully made a socket
		printf("Server listening on file descriptor %i and host %s\n", fd, host_info);
		break;
	}

	return fd;
}

/*
 * Send data to a server
 * @param host ip address to send data to
 * @param port port or protocol the server is running see /etc/services or services(2)
 * @return the response from the server
 */
int request(struct Http http, char *response) {
	int fd = create_connection(http); 
	if (fd == -1) return fd;

	//Format Request
	char *startline = "%s %s %s"; // method, route, version;
	char *payload_format = "%s %s %s\r\n%s\r\n\r\n";
	char *req_format = "%s %s %s\r\n%s%s\r\n\r\n";
	char req[LARGE];
	// Todo MERGE to one function so don't have to pass size twice
	int ret = snprintf(req, sizeof(char) * LARGE, 
			   req_format, 
			   http.method,
			   http.route,
			   http.version,
		       http.headers,
			   http.payload);
	handle_snprintf(ret, sizeof(char) * LARGE);


	// Send the payload to the host
	ret = write_request(fd, req);
	if (ret < 0) {
		close(fd);
		return -1;
	}

	while (read_recv(fd, response, sizeof(char)*MEDIUM, 0) != NULL) {
		printf("%s\n", response);
	}

	// Successs 
	close(fd);
	return 1;
}

/**
 * http.host the url to connect to
 * http.port the port the url is hosted on
 * return -1 on failure 
 * return a descriptor [0, inf) referencing the socket
 */
int create_connection(struct Http http) {
	struct addrinfo config;
	memset(&config, 0, sizeof(struct addrinfo));
	config.ai_family = PF_UNSPEC;
	config.ai_socktype = SOCK_STREAM;
	// No flags are used

	// Get a list of IP address and ports 
	struct addrinfo *hosts;
	int err = getaddrinfo(http.host, http.port, &config, &hosts);
	if (err != 0) {
		fprintf(stderr,	"getaddrinfo failed: %s\n", 
			gai_strerror(err));
		return -1;
	}

	// Connect to the first IP possible
	char *response = malloc(sizeof(char) * LARGE);
	int fd = -1;
	while (hosts) {
		char host_info[MEDIUM];
		sockaddr_tostring(hosts->ai_addr, host_info);
		fd = socket(hosts->ai_family, hosts->ai_socktype, hosts->ai_protocol);
		if (fd < 0) {
			printf("unable to make socket %i for %s because %s\n", 
			       fd, host_info, strerror(errno));
			perror("socket error");
			hosts = hosts->ai_next;
			continue;
		}


		if ((connect(fd, hosts->ai_addr, hosts->ai_addrlen)) < 0) {
			printf("socket %i connect to %s failed because %s\n", fd, host_info, strerror(errno));
			perror("connect error");
			hosts = hosts->ai_next;
			close(fd);
			fd = -1;
			continue;
		}

		printf("Successfully made a connection to host %s\n%s\n", 
		       http.host, 
		       sockaddr_tostring(hosts->ai_addr, host_info));
		break;
	}
	return fd;
}

/*
 * Sets and returns the buffer with the payload from the server
 * @param fd is the file descriptor
 * @param response is written to buffer 
 * @return the buffer or NULL on error
 */
char* read_request(int fd, char *response) {
	int bytes_read = 0;
	if ((bytes_read = read(fd, response, LARGE * sizeof(char))) == -1) {
		perror("read");
		close(fd);
		return NULL;
	} else if (bytes_read == 0) {
		printf("End of file reached\n");
		snprintf(response, sizeof(char) * LARGE, "EOF");
		return response;
	} else {
		response[bytes_read] = '\0';
		return response;
	}
}

char *read_recv(int socket, char *buffer, size_t length, int flags) {
	int ret = recv(socket, buffer, length-1, flags);
	if (ret == 0) {
		printf("peer on socket %i closed connection\n", socket);
		return NULL;
	}
	if (ret == -1) {
		perror("recv");
		return NULL;
	}
	buffer[ret] = '\0';
	return buffer;
}

/**
 * Reads data from a socket
 * Overwrites the buffer and returns it
 * @param fd is a socket file descriptor 
 * @returns the buffer
 */
char* recv_request(int fd, char *buffer, size_t length, int flags) {
	int bytes_recv = recv(fd, &buffer, length, flags);

	if (bytes_recv == -1) {
		printf("pid %i socket %i recv failed because %s\n",
		       getpid(), fd, strerror(errno));
		perror("recv error");
		return NULL;
	} 

	if (bytes_recv == 0) {
		printf("pid %i socket %i recv done\n", getpid(), fd);
		return 0;
	}

	// badass
	buffer[bytes_recv] = '\0';
	return buffer;
}

// Send data to client 
int write_request(int fd, char *buffer) {
	printf("Sending request...\n%s\n", buffer);
	int bytes = 0;
	if((bytes = write(fd, buffer, strlen(buffer) * sizeof(char))) == -1) {
		perror("write");
		return -1;
	}
	if (bytes < strlen(buffer) * sizeof(char)) {
		fprintf(stderr, "Did not print entire buffer\n");
		return -1;
	}
	printf("Done sending request\n");
	return 0;
}


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
		fprintf(stderr, "inet_ntop failure %s", strerror(errno));
		print_callstack();
	}


	char *text = "struct sockaddr {\n\tfamily: %i %s\n\taddress: %s\n\tport: %i\n}";
	int ret = snprintf(buffer, sizeof(char) * MEDIUM, text,
			   sockaddy->sa_family, family,
			   host_str,
			   htons(port));
	handle_snprintf(ret, sizeof(char) * MEDIUM);

	return buffer;
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


