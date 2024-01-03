# Addy

## Run
Assumes machine running UNIX system, macOS, or linux distribution.
Will create a server listening on http://localhost:3000 by default.
Will create a client that sends a request to a server and prints out the response.

```
git clone https://github.com/thsmale/addy
cd addy

chmod u+x client server
./client
./server
```

## Usage
Create an HTTP server and return a file descriptor to listen on.
```
#include <addy.h>
int fd = start_server("localhost", "3000")
```

Make a HTTP request.
Note this request returns the google homepage. 
A better usage is making a request to an API that returns csv data.
```
#include <addy.h>

// Configure a HTTP object
struct Http http;
http.method = "GET";
http.route = "/index.html";
http.host = "www.google.com";
http.port = "80";

char *response = request(http);
printf("%c\n", response);
```

## Test
These test that the addy server or client is HTTP compatible.
One can configure the unit tests by setting the port and hostname like this.
```
$ export ADDY_HOST=localhost
$ export PORT=3000
```

### hapi
hapi is web application server framework for HTTP that can create HTTP requests and receive HTTP requests.

```
./server
cd test/hapi
# Testing the addy server
npm run test-server
```