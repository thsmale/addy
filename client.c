#include "addy.h"

struct Http {
	char *method;
	char *route;
	char *version;
	char *host;
	char *port;
};

int main() {
	struct Http http;
	http.method = "GET";
	http.route = "/index.html";
	http.version = "HTTP/1.1";
	http.host = "www.google.com";
	http.port = "80";
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
	for (int i = 0; i < 10; ++i) {
	char *response = request(http.host, http.port, payload);
	if (response == NULL) {
		print_callstack();
		return -1;
	}
	printf("response: %s\n", response);
	}
	return 0;
}

