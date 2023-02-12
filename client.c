/*
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
*/


#include "addy.h"

int main() {
	struct addrinfo host_config;
	memset(&host_config, 0, sizeof(struct addrinfo));
	host_config.ai_family = PF_UNSPEC;
	host_config.ai_socktype = SOCK_STREAM;
	host_config.ai_flags = AI_PASSIVE;

	struct addrinfo *hosts;
	char *host = "localhost";
	char *port = "8443";
	int err_num = 0;
	if ((err_num = getaddrinfo(host, port, &host_config, &hosts) != 0)) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(err_num));
		return -1;
	}

	int fd = 0;
	while (hosts) {
		// fd being any value in the range (-infinity, 0] will equal false
		if ((fd = socket(hosts->ai_family, hosts->ai_socktype, hosts->ai_protocol))) {
			char buf[MEDIUM];
			printf("unable to make socket for %s", sockaddr_tostring(hosts->ai_addr, buf));
			perror("socket error: \n");
			hosts = hosts->ai_next;
			continue;
		}


		if ((connect(fd, hosts->ai_addr, hosts->ai_addrlen)) < 0) {
			char buf[LARGE];
			char sockaddr_string[MEDIUM];
			int ret = snprintf(buf, 
					   sizeof (char) * LARGE, 
					   "socket unable to connect to %s  because: \n", 
					   sockaddr_tostring(hosts->ai_addr, sockaddr_string));
			handle_snprintf(ret, sizeof(char) * LARGE);
			char *buf_ptr = buf;
			perror(buf_ptr);
			hosts = hosts->ai_next;
			close(fd);
			continue;
		}

		char buf[MEDIUM];
		printf("Successfully made a connection to host %s\n", sockaddr_tostring(hosts->ai_addr, buf));
		break;
	}

	close(fd);

	return 0;
}

