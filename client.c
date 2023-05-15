#include "addy.h"

int main() {
	struct Http http;
	http.method = "GET";
	http.route = "/";
	http.version = "HTTP/1.1";
	http.host = "localhost";
	http.port = "3000";
	http.headers = "host: localhost:3000";
	http.payload = "";
	
	char *response = request(http);
	if (response == NULL) {
		print_callstack();
		return -1;
	}
	printf("response: %s\n", response);
	return 0;
}

