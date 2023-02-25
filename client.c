#include "addy.h"

int main() {
	char *response = request("www.google.com", "80", "");
	if (response == NULL) {
		print_callstack();
		return -1;
	}
	printf("response: %s\n", response);
	return 0;
}

