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

// struct client, server, request, 
struct Addy {
};

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

/**
 * server and client functions
*/
int start_server(char *host, char *port);
char* request(char *host, char *port, char *payload);
char* recv_request(int fd, char *buffer);
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
