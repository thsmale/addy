#include "addy.h"

int main() {
	int fd = start_server("localhost", "8000");
	if (fd < 1) {
		printf("Failed to create server\n");
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
	char host[MEDIUM]; 
	while (1) {
		if((new_fd = accept(fd, &new_connection, &address_len) == -1)) {
			perror("accept: \n");
			continue;
		}
		printf("Accepted new conection from %s\n", sockaddr_tostring(&new_connection, host));
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

}
