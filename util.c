#include "addy.h"

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


