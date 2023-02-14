#include <unistd.h>
#include <stdio.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <string.h>

void getlimit(int stat_type, struct rlimit *limits) {
	if (getrlimit(stat_type, limits) == -1) {
		perror("getrlimit");
	}
}

void stats() {
	/**
	 * Only sudo can raise rlim_max (over clocking?)
	 * rlim_cur range is (rlim_min, 0) (0, rlim_max)
	 * struct rlimit {
	 * 	rlim_t rlim_cur
	 * 	rlim_t rlim_max
	 * }	
	 */
	struct rlimit limits;
	printf("CSV TABLE\n");
	printf(
	       "Limits on consumption of system resources by the current process and each process it creates\n"		);
	printf("----|------------------------\n");
	printf("current, max, unit, description\n"); //"" /0 for no unit NOT "none"
						     //
	getlimit(RLIMIT_CORE, &limits);
	printf("%llu, %llu, bytes, largest *core* file size that may be created\n", 
	       limits.rlim_cur, limits.rlim_max); 

	getlimit(RLIMIT_CPU, &limits);
	printf("%llu, %llu, seconds, max CPU used by process <pid>\n", limits.rlim_cur, limits.rlim_max);

	getlimit(RLIMIT_DATA, &limits);
	printf("%llu, %llu, bytes, max data segement for process <pid>\n", limits.rlim_cur, limits.rlim_max);


	getlimit(RLIMIT_FSIZE, &limits);
	printf("%llu, %llu, bytes, largest file size that may be created\n", limits.rlim_cur, limits.rlim_max);

	/*
	 * mlock(2) related
	 if (getrlimit(MEMLOCK, &limits) == -1) {
	 perror("getrlimit");
	 }
	 */

	getlimit(RLIMIT_NOFILE, &limits);
	printf("%llu, %llu, decimal, open files for this process <pid>\n", limits.rlim_cur, limits.rlim_max);

	getlimit(RLIMIT_NPROC, &limits);
	printf("%llu, %llu, decimal, simulateneous processes for this <pid>\n", 
	       limits.rlim_cur, limits.rlim_max);

	getlimit(RLIMIT_RSS, &limits);
	printf("%llu, %llu, bytes, physical memory allocated to processes <pid>\n", 
	       limits.rlim_cur, limits.rlim_max);

	getlimit(RLIMIT_STACK, &limits);
	printf("%llu, %llu, bytes, max stack size for processes <pid>\n", 
	       limits.rlim_cur, limits.rlim_max);
}


/*
 * from Beej guide simple stream server
 */
void sigchld_handler() {
	// waitpid() might overwrite errno so save and restore it
	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;

}	

/**
 * Try writing it three different ways
 * stdout fd via printf
 * stdout fd via write
 * write to file via fwrite
 */
void out() {
	printf("uno\n");

	char buf[4096];
	sprintf(buf, "PICKLE\n");
	write(1, buf, strlen(buf));

	FILE *fd = fopen("fork.out", "w");
	if (fd == NULL) 
		perror("fopen fork.out");
	int nitems = 0;
	if((nitems = fwrite(buf, sizeof (char), strlen(buf), fd) < sizeof(char)*strlen(buf) < strlen(buf))) {
		perror("fwrite");
	}
	fclose(fd);
}

int main() {
	/**
	 * Try writing it three different ways
	 * stdout fd via printf
	 * stdout fd via write
	 * write to file via fwrite
	 */
	printf("uno\n");

	char buf[4096];
	sprintf(buf, "PICKLE\n");
	write(1, buf, strlen(buf));

	
	FILE *fd = fopen("fork.out", "w");
	if (fd == NULL) 
		perror("fopen fork.out");
	int nitems = 0;
	if((nitems = fwrite("fwrite\n", sizeof (char), strlen(buf), fd)) /*< sizeof(char)*strlen(buf))*/ < strlen(buf)) {
		perror("fwrite");
	}
	fclose(fd);

	stats();


	struct sigaction sa;	
	sa.sa_handler = sigchld_handler; // zombie killer
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		return -1;
	}

	int processes = 0;
	for (int i = 0; i < 1; ++i) {
		// process which returns the child pid process
		pid_t pid = fork();
		if (pid < 0) {
			fprintf(stderr, "fork failed %s", strerror(errno));
			perror("fork");
			break;
		}
		if (pid == 0) {
			// Child process
			processes += 1;
			return 0;
		} 
		// Parent 
		// Or return from child process
		//printf("I am the parent process pid = %i\n", pid);
		int status = 0;
		int result = waitpid(pid, &status, 0);
		printf("nuts\n");
		if (WIFEXITED(status)) {
			printf("dollar");
			printf("Process %i exited peacefully with status %d\n", pid, WEXITSTATUS(status));
			printf("csv, %i, %d, %ld\n", pid, WEXITSTATUS(status), time(0));
		} else {
			printf("Process %i terminated abonrmally\n", pid);
		}
	}
	printf("Number of processes at exit %i\n", processes);
	return 0;
}
