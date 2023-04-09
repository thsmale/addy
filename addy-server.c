#include "addy.h"
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
	int fd = start_server("localhost", "8000");
	if (fd < 1) {
		printf("Failed to create server\n");
		return -1;
	}
	/**
	 * Accept
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
	char client[MEDIUM]; 
	int new_fd; 
	while (1) {
		int new_fd = accept(fd, &new_connection, &address_len);
		if (new_fd == -1) {
			perror("accept failed");
			continue;
		}

		sockaddr_tostring(&new_connection, client);
		printf("Server accepted new conection from %s\n", client);

		pid_t pid = fork();
		if (pid < 0) {
			fprintf(stderr,
				"Failed to start process for client %s because %s", client, strerror(errno));
			continue;
		} 

		if (pid == 0) {
			// Receive request from client 
			char response[LARGE];
			while(read_recv(new_fd, 
					response,
					sizeof(char)*LARGE,
					0) != NULL) {
				printf("%s\n", response);
			}
		}

		// Parent 
		// Or return from child process
		int status = 0;
		int result = waitpid(pid, &status, 0);
		if (WIFEXITED(status)) {
			printf("Process %i exited peacefully with status %d\n", pid, WEXITSTATUS(status));
			// Collect up to a 1000, then more
			printf("CSV-----|----------\n");
			printf("pid, exit_status, time 64b\n");
			printf("%i, %d, %ld\n", pid, WEXITSTATUS(status), time(0));
		} else {
			printf("Process %i terminated abonrmally\n", pid);
		}
	}
	if (close(fd) == -1) perror("close");
	return 0;
}
