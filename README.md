# Addy

## Run
Assumes machine running UNIX system, macOS, or linux distribution.
Will create a server listening on http://localhost:3000 by default.
Will create a client that sends a request to a server and prints out the response.

```
git clone https://github.com/thsmale/addy
cd addy/examples

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
struct Http options;
options.method = "GET";
options.route = "/index.html";
options.host = "www.google.com";
options.port = "80";

struct Http response;
int response = request(http, &response);
printf("Response payload: %s\n", response.payload);
```

## Logging
One can adjust the `LOG_LEVEL` to control what kinds of logs they see in stdout and stderr from the addy server or request framework.
|            |       |       |      |      |       |
|:-----------|-------|-------|------|------|-------:|
| **Level:** | TRACE | DEBUG | INFO | WARN | ERROR |
| **Value:** | 10    | 20    | 30   | 40   | 50    |

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
# Testing the HTTP addy server
npm run test-server
# Testing HTTP addy requests
node server.js
chmod u+x test-client
./test-client
```