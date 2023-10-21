#include "addy.h"

int main() {
	struct Http options;
	options.method = "GET";
	options.route = "/";
	options.version = "HTTP/1.1";
	options.host = "localhost";
	options.port = "3000";
	options.headers = "host: localhost:3000";
	options.payload = "";

	char response[MEDIUM];
	int success = request(options, response);
	if (!success) {
		print_callstack();
		return -1;
	}
	printf("response: %s\n", response);
	return 0;
}

