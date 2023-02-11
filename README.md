# Addy
1. An example HTTPS server
	- Based off Unix networking
2. A server using a custom networking protocol
	- Every type is an object
		- Using snake_case
	- Built off on top of existing Unix networking
	- Built in HTTP headers for request and validations
		- Ensures you are following best practices
3. Tests: These will be made into an interactive website to test
	- Each section will be a question
	- User will provide an answer

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
