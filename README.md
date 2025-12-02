# Addy

## Run
Assumes machine running UNIX system, macOS, or linux distribution.
Will create a addy web server listening on http://localhost:3000 by default.
Will create a addy http client that sends a request to the addy web server and prints out the response.

```
git clone https://github.com/thsmale/addy
cd addy/examples

chmod u+x client.sh server.sh
./server.sh
./client.sh
```

## Usage
Create an HTTP server and return a file descriptor to listen on.
```c
#include <addy.h>
int fd = start_server("localhost", "3000")
```

Make a HTTP request.
Note this request returns the google homepage. 
A better usage is making a request to an API that returns csv data.
```c
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
[hapi](https://hapi.dev/) is web application server framework for HTTP that can create HTTP requests and receive HTTP requests.
Since this is a reliable framework, it is used to test the functionality of the addy HTTP web server and requests.

```
# Start a test addy server to receive HTTP requests
cd examples
./server.sh
# Testing the addy HTTP web server
cd test/hapi
npm i
npm run test-server
# Testing addy HTTP requests
node server.js
chmod u+x test-client.sh
./test-client.sh
```