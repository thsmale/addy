# Addy
1. An example HTTPS server
	- Based off Unix networking
2. A server using a custom networking protocol
	- Every type is an object
		- Using snake_case
	- Built off on top of existing Unix networking
	- Built in HTTP headers for request and validations
		- Ensures you are following best practices
	- Test this server with nodejs frameworks, python frameworks
	- Able to create requests
	- Able to create servers
	- Uses standardized return system
		- [-Infinity, 0] error
		- (1, Infinity) success
3. Tests: These will be made into an interactive website to test
	- Each section will be a question
	- User will provide an answer
	- Like a like bar but 0 and 1 which indicates if answer true or false
	- This site will be hosted on the web
	- Its API will be build in Addy

```
gcc client.c addy.c
gcc server.c
```

No .h files because this is C not h
Need a solution to include headers
	- Two files cannot both include the same header file
	- That like causes an stack overflow
	- Just take what you need im the file, get rid of what we dont
	- **They all share the same .h file**
/**
  * TS
  *next* 
  &*-> address location of type addyinfo

  ***addy***
  URInary track infection
  host
  base_url
  endpoints

  ***addylen***
  ###### of characters

  ***protocol***
  HTTPS
  HTTP
  FTP
  TCP
  UDP
  TLS 
  	SYN->SYN ACK->ACK(hi)->cert->key
	Developed for HTTP
	Replaced SSL
	Specify TLS version
	  * default to highest one available
	Decide on cipher suites
	server public key and CA 
	Generate session keys after handshake is complete
  SSL
  fenny


  ***FAMILY***
  brother
  sister
  parents

  ***FLAGS***
  red
  green
  blue
*/

/**
	  * man getaddrinfo 
	  * @var ai_family: Only accept protocols supported by the os
	  * @var ai_socktype: Stream or datagram (ssh && HTTP use stream sockets)
	   	- SOCK_STREAM uses TCP: Transmission Control Protocol
		- SOCK_DGRAM uses UDP: may arrive out of order
			- Just ignore em or approximate it
	  * @var flags: first 10 bits matter
	  	- Which is first?
		- If you aint first you last
		- Should fit in 8 bits
		AI_ADDRCONFIG System decides IVP4 or IPV6
		AI_ALL 
		AI_CANNONAME
		AI_NUMERICHOST hostname is IPv4 or IP6 or DNS should be performed
		AI_NUMERICSERV
		AI_PASSIVE 
			1. TCP: returned socket address structure will use bind
				- hostname will be null pointer
				- IP address will be set 
					- INADDR_ANY for IPv4
					- IN6ADDR_ANY_INIT for IPv6
			0. UDP: returned socket address struct will use connect, sendto, sendmsg
				- if hostname is null pointer && AI_PASSIVE === 0
					- IP address will be set to loopback addresss 


		AI_V4MAPPED
		AI_V4MAPPED_CFG
		AI_DEFAULT
		AI_UNUSABLE	
	  */

	/**
	 * Beej Guide
	 * "anyone who says TCP never arrives out of order I wont listen lalala"
	 * AF = address family
	 * INET = internet
	 * memset(&hints, 0, sizeof(addrinfo));
	 * hints.ai_family = AF_INET6; 
	 * hints.ai_socktype = SOCK_DGRAM;
	 * hints.ai_flags = AI_PASSIVE;
	 */

	/**
	 * man page
	 * getaddrinfo gets a list of IP addresses and port names for hostname and servname
	 * 	- hostname & servname are null pointer or null terminated strings
	 * 		  TS
	 * 		  Neither can be null
	 * 		  One must be string
	 * 		  Both can be string
	 * 		  Neither can't be strings
	 * 	- hostname is valid host name or string IPv4/6 addresss
	 * 	- servname is port number of service name listed in /etc/services services(5)
	 TS
	 char *host = "addy.address"
	 Map to /etc/hosts localhost
	 Now go to addy.address in chrome browser
	 Useful if TLS cert != localhost and running locally
	 * replaces gethostbyname and getservbyname()
	 */
