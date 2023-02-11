#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#define LARGE 2048

/**
 * Print the callstack
 * Useful for debugging
 * man backtrace for more info
 */

void foo();
void print_stack();

int main () {
	foo();
}

void foo() {
	printf("oh no something wen't wrong\n");
	print_stack();
}

void print_stack() {
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
