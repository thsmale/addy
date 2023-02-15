#include "addy.h"

int main() {
	struct addrinfo host_config;
	memset(&host_config, 0, sizeof(struct addrinfo));
	host_config.ai_family = PF_UNSPEC;
	host_config.ai_socktype = SOCK_STREAM;
	//host_config.ai_flags = AI_PASSIVE;

	struct addrinfo *hosts;
	char *host = "localhost";
	char *port = "8000";
	int err_num = 0;
	if ((err_num = getaddrinfo(host, port, &host_config, &hosts) != 0)) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(err_num));
		return -1;
	}

	int fd = 0;
	while (hosts) {
		char host_info[MEDIUM];
		sockaddr_tostring(hosts->ai_addr, host_info);
		// fd being any value in the range (-infinity, 0) will equal false
		if ((fd = socket(hosts->ai_family, hosts->ai_socktype, hosts->ai_protocol)) < 0) {
			printf("unable to make socket %i for %s because %s\n", fd, host_info, strerror(errno));
			//strerror(errno);
			perror("socket error");
			hosts = hosts->ai_next;
			continue;
		}


		if ((connect(fd, hosts->ai_addr, hosts->ai_addrlen)) < 0) {
			printf("socket %i connect to %s failed because %s\n", fd, host_info, strerror(errno));
			perror("connect error");
			hosts = hosts->ai_next;
			close(fd);
			continue;
		}

		char buf[MEDIUM];
		printf("Successfully made a connection to host %s\n", sockaddr_tostring(hosts->ai_addr, buf));

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
		int ret = recv(fd, buffer, sizeof(char)*LARGE-1, 0);

		if (ret < 0) {
			perror("recv error");
			close(fd);
			hosts = hosts->ai_next;
			continue;
		} 

		if (ret == 0) {
			//printf("pid %i socket %i recv done\n", getpid(), new_fd);
			printf("recv done\n");
			close(fd);
			hosts = hosts->ai_next;
			continue;
		}

		buffer[ret] = '\0';
		//printf("pid %i socket %i recv %s from client\n", getpid(), fd, buffer);
		printf("%s\n", buffer);


		break;
	}

	close(fd);

	return 0;
}

