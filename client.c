#include "addy.h"

int main() {
	char *response = request("localhost", "8000", "ping");
	if (response == NULL) {
		print_callstack();
		return -1;
	}
	printf("response: %s\n", response);
	return 0;
}

