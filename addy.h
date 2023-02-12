#include <stdlib.h>
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <execinfo.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/socket.h>

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
 * Config client
 */
//int create_client(char *host, char *port, int flags);

char* sockaddr_tostring(struct sockaddr *addy, char *ret);
int set_socket(struct addrinfo *hosts);

/**
 * UTILS file
*/
void print_callstack();
// Print callstack and warning message
void handle_snprintf(int ret, size_t size);

/**
 * LOG file
 * For debugging purposes
*//**

 * Prints the contents of teh struct addrinfo
 * Output is similar to JSON like structure
 */
void print_host(struct addrinfo *addy_info);
