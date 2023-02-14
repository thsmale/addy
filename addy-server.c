#include "addy.h"

void zombie_handler() {
}

int main() {
	int fd = start_server("localhost", "8000");
	if (fd < 1) {
		printf("Failed to create server\n");
		return -1;
	}

	/**
	 * stuct sigaction {
	 * 	union __sigaction_u __sigaction_u  // call back function signal handler
	 * 	sigset_ sa_mask //signal mask
	 * 	int sa_flags 
	 */
	struct sigaction sa;
	sa.sa_handler = zombie_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
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
	char client[MEDIUM]; 
	int processes = 0;
	while (1) {
		if((new_fd = accept(fd, &new_connection, &address_len) == -1)) {
			perror("accept failed");
			continue;
		}

		sockaddr_tostring(&new_connection, client);
		printf("Server accepted new conection from %s\n", client);

		pid_t pid = fork();
		if (pid < 0) {
			printf("Failed to start process for client %s because %s", client, strerror(errno));
			continue;
		} 
		else if (pid == 0) {
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

			if (ret < 0) {
				printf("pid %i socket %i recv failed because %s\n",
				       getpid(), new_fd, strerror(errno));
				perror("recv error");
				exit(-1);
			} 

			if (ret == 0) {
				printf("pid %i socket %i recv done\n", getpid(), new_fd);
				close(new_fd);
				exit(1);
			}

			buffer[ret] = '\0';
			printf("pid %i socket %i recv %s from client\n", getpid(), new_fd, buffer);
		} 
	}
	close(fd);
	return 0;
}
