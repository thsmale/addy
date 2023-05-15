#include "addy.h"

int main() {
	/**
	 * TODO: 
	 * Pass in HTTP struct to request
	 * Add payload to Http struct
	 * Add response to HTTP
	 * Add formatters to the HTTP request function
	 */
	struct Http http;
	http.method = "GET";
	http.route = "/index.html";
	http.version = "HTTP/1.1";
	http.host = "www.google.com";
	http.port = "80";

	// char * response = request(http);
	
	//char *startline = "%s %s %s", method, route, version;
	char *startline = "%s %s %s";//, method, route, version;
	char *payload_format = "%s %s %s\r\n%s\r\n\r\n";
	char payload[MEDIUM];
	// Todo MERGE to one function so don't have to pass size twice
	int ret = snprintf(payload, sizeof(char) * MEDIUM, 
			       payload_format, 
			       http.method, http.route, http.version,
			       http.host);
	handle_snprintf(ret, sizeof(char) * MEDIUM);
	http.payload = payload;
	for (int i = 0; i < 10; ++i) {
	//char *response = request(http.host, http.port, payload);
	char *response = request(http);
	if (response == NULL) {
		print_callstack();
		return -1;
	}
	printf("response: %s\n", response);
	}
	return 0;
}

