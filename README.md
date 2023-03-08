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
```c
#include <addy.h>
int server = start_server("localhost", "3000")
char *response = request("localhost", "3000", "/healthcheck");
printf("%c\n", response)
```

### Notes

1. An example HTTPS server
	- Based off Unix networking
2. A server using a custom networking protocol
	- Built off on top of existing Unix networking
	- Built in HTTP headers for request and validations
		- Ensures you are following best practices
	- Test this server with nodejs frameworks, python frameworks
	- Able to create requests
	- Able to create servers
	- Uses standardized return system
		- [-Infinity, 0] error
		- (1, Infinity) success
	- Has metrics to see sockets open, cpu usage, etc
		/**
		 * Child prcess
		 * Add metrics from getrlimit(2)
		 * To print out real time stats about server
		 * An array of connections
		 * struct connection 
		 * 	- pid
		 * 	- rlimit information
		 */
	- start_server returns a Server struct
	- you can add functions to the server struct


* man getaddrinfo 
* ai_family: Only accept protocols supported by the os
* ai_socktype: Stream or datagram (ssh && HTTP use stream sockets)
	- SOCK_STREAM uses TCP: Transmission Control Protocol
	- SOCK_DGRAM uses UDP: may arrive out of order
		- Just ignore em or approximate it
* flags: first 10 bits matter
	- AI_ADDRCONFIG System decides IVP4 or IPV6
	- AI_ALL 
	- AI_CANNONAME
	- AI_NUMERICHOST hostname is IPv4 or IP6 or DNS should be performed
	- AI_NUMERICSERV
	- AI_PASSIVE 
		- TCP: returned socket address structure will use bind
			- hostname will be null pointer
			- IP address will be set 
				- INADDR_ANY for IPv4
				- IN6ADDR_ANY_INIT for IPv6
		- UDP: returned socket address struct will use connect, sendto, sendmsg
			- if hostname is null pointer && AI_PASSIVE === 0
				- IP address will be set to loopback addresss 


	- AI_V4MAPPED
	- AI_V4MAPPED_CFG
	- AI_DEFAULT
	- AI_UNUSABLE	

 * man page
 * getaddrinfo gets a list of IP addresses and port names for hostname and servname
  	- hostname & servname are null pointer or null terminated strings
  		  - Neither can be null
  		  - One must be string
  		  - Both can be string
  		  - Neither can't be strings
  	- hostname is valid host name or string IPv4/6 addresss
  	- servname is port number of service name listed in /etc/services services(5)

- char *host = "addy.address"
- Map to /etc/hosts localhost
- Now go to addy.address in chrome browser
- Useful if TLS cert != localhost and running locally
- replaces gethostbyname and getservbyname()

