#include "addy.h"

int main() {
	struct Http options;
	char *host = getenv("ADDY_HOST");
	char *port = getenv("PORT");
	if (!host) {
		host = "localhost";
	}
	if (!port) {
		// set port to default 3000
		port = "3000";
	}
	options.method = "GET";
	options.route = "/";
	options.version = "HTTP/1.1";
	options.host = host;
	options.port = port;
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

