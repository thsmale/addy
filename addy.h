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

// struct client, server, request, 
struct Addy {
};

struct Http {
	char *method;
	char *route;
	char *version;
	char *host;
	char *port;
	char *payload;
	//struct Http *response;
};

/**
 * server and client functions
*/
int start_server(char *host, char *port);
char* request(struct Http http);
int create_connection(struct Http http);
char* recv_request(int fd, char *buffer, size_t length, int flags);
char *read_recv(int socket, char *buffer, size_t length, int flags);
char* read_request(int fd, char *buffer);
int write_request(int fd, char *buffer);

/**
 * UTILS file
 * print_callstack: stack trace for debugging
 * hanlde_snprintf: print callstack and warning message
 * sockaddr_tostring: convert sockaddr struct to string  for debugging
*/
void print_callstack();
void handle_snprintf(int ret, size_t size);
char* sockaddr_tostring(struct sockaddr *addy, char *ret);

/**
 * LOG file
 * For debugging purposes
*//**

 * Prints the contents of teh struct addrinfo
 * Output is similar to JSON like structure
 */
void print_host(struct addrinfo *addy_info);
