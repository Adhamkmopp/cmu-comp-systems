# Chapter 11: The Web

## The Client-Server Model

An application condists of *both* one or more client processes and a single server process runinng on "hosts" (machines), and they do not have to be seperate. Requests and responses are sent back and forth between client and server, completing the concept of a client-server model.

## Network

A network is different things depending on the perspective. To a host, a network is another I/O device connected through a network adapter. A network is also a geographically linked set of computers (hosts/machines/whatever). The building blocks go as follows:

1.  Segment: smallest local area of a bunch of wires connecting hosts to a hub. A hub copies every bit it receives through each port to every other port (why? can't the segment handle directing the signal to its correct destination port?). Every host can rudely eavesdrop on any outgoing singal with some unknown justification. Signals flow through ethernet adapters that each carry a unique 48 bit identifier.

2.  Frames: every chunk of data is called a frame, with a payload and a header describing the destination and source as well as the length of the frame itself. Only the destination can read it. This is the exact definition of a packet as well, except that the packet stores the IP address of the sender and destination.

3.  Bridges: conneting multiple segments requires a bridge that selectively excludes singals from segments that have a destination within the local segment to save up on bandwidth (implying that the bridge has that same peculiar behaviour of copying every bit it resceives to every port, but only when necessary in this case.).

The LANs above at the highest level are connected through routers, which themselves connect to each network via its own adaptor (and so a unique identifier for each connected network as well). A frame destined to a different LAN from the source is encapsulated within a packet containing the IP address of the sender and receiver, and it is further encapsulated within a frame header addrssed to the router with the sender and the router's unique 48-bit identifier. The router receives the signal, figures out the unique identifier of the receiver from the IP stored in the packet header (via routing table), strips off the frame header, and assigns the unique identifier of the sender and receiver to a new frame header and sends it on its way. All the functions performed here are done either by the kernel or a piece of *protocol software*.

## The Global IP Internet

TCP/IP (Transmission Control Protocol/Internet Protocol): communication between hosts "..is a mix of socket interface functions (system calls) and Unix I/O functions working in TCP/IP":

1.  Every host has its own unique 32 bit IP address (IPv4 anyway), usually presented in dot-decimal form (each byte seperated by a dot).
2.  IP adresses are mapped to unique internet domain names.
3.  Data is transferred and stored in big-endian mode.
4.  IP addresses: an unsigned 32 bit integer organized in dot-decimal notation and sent to the network in big-endian mode:

~~~c
struct in_addr{
    uint32_t s_add; // for historical reasons
}
~~~

5.  Domain Names: just human-readable friendly names mapped to IP addresse. The hierarchy is read from right to left, with .com being a first-level domain for example and downwards towards www. There may be multiple domain names mapped to the same IP or multiple IPs given to the same domain name.

6.  Sockets: A socket is an end point of a connection with an address consisting of the IP address followed by a 16 bit port number (IP address:port). Ports and addresses are often associated together if well-known such as http and port 80, but a host creates its own port on the fly as assigned by the kernel (ephemeral port) on a data request as a client, not a server. Together they form a client-server pair, or connection:

(cliaddr:cliport, servadr:servport)


## The Socket Interface

The socket interface is a set of function used in manipulating sockets, and thereby connections. A socket itself is an open file with a descriptor from a processes's perspective. A socket adress structure is 16 bytes and contains the protocol family, port number, IP address and an 8 byte padding.


~~~c
/* For casting sockadd_in as a generic type in socket functoins */
typedef struct sockaddr SA;

/* IP socket address structure */
struct sockaddr_in{
    uint16_t sin_family; /* Protocol family */
    uint16_t sin_port; /* Port number in network byte order */
    struct in_addr sin_addr; /* IP address in network byte order */
    unsigned char sin_zero[8]; /* Padding */
}

/* Generic socket address structure (for socket interface functions) */
struct sockadd {
    uint16_t sa_family; /* Protocol family */
    char sa_data[14]; /* Address data */
}
~~~

### The Client's Perspective

To begin with, a socket descriptor needs to be created with the correct protocol, and then mapped to the socket address structure. A lot like opening a file locally, except that the file path is equivalent to the sock address in this case and the operation is split into two seperate steps.

~~~c
int socket(int domain, int type, int protocol);
clientfd = Socket(AF_INET, SOCK_STREAM,0);

int connect (int clientfd, const struct sockaddr *addr, socklen_t addlen);
~~~

Domain is the protocol family (like IPv4 or IPv6), the type defines the connection type (fully duplex, reliable, etc) and the protocol defines the specific protocol if available (no idea what to do with this yet). It rerturns a partially opened *client descriptor* that will be used to communicate with the server or the client.

It seems that to fully open the connection the clientfd set up with the correct protocol must be passed to the connect function, along with the server sock address and size of the sock address struct (isn't it always 16 bytes?). The connection is then set up if successful (0 on return).

## ### The Server's Perspective

The function associates the server's sock address to the sockfd descriptor; it *binds* a process to a specific port and expects it to wait and listen for a request. Like connect but flowing in the opposite direction.

~~~c
int bind (int sockfd, const struct sockaddr *addr, socklen_t addlen);
~~~

Additionally, the listen function converts the sockfd from an *active socket* to a *listening socket*  that can accepts future requests from clients. Created once and lasts a lifetime on the server.

~~~c
int listen(int sockfd, int backlog);
~~~

The listening socket now set, the function waits for a request from a client and fills in the socket adress of the client in sockaddr addr and returns a *connected descriptor* that can be used to communicate with the client via Unix I/O functions.

~~~c
int accept(int listenfd, struc sockaddr *addr, int *addrlen);
~~~

The distingsion between a listening descriptor and a connected descriptor allows for accepting client connections while still listening to new incoming connections.


## Host and Service Conversion

To recap, a client must first have some a priori information on the server it wishes to connect to, and the kind of service it expects to find. It builds a file descriptor using the socket function, then a socket address structure (sockaddr_in) is mapped to the client descriptor. From the server's side, a descriptor must be made associated with the server's own socket address structure, then mapped again as a new listening file descriptor that can accept requests, with the accept function that finally returns a *connected descriptor* used in communication with the client; the parallel to the client's *client descriptor*.

### getaddinfo Function
To sidestep the requirement of specifically setting the protocol for each socket, the getaddrinfo allows for code independant of protocol used.

~~~c

int getaddrinfo(const char *host, const char *service, const struct addinfo *hints, struct addinfo **results);

void freeaddinfo(struct addinfo *result);

const char *gai_strerror(int errcode);

struct addrinfo{
    int ai_flags; /* OR'd finer controls such as AI_ADDRCONFIG | AI_PASSIVE (for severs; set host to NULL) */
    int ai_family; /* IPv4 or IPv6 (AF_INET) */
    int ai_socktype; /* Second argument to sock function */
    int ai_protocol; /* Third argument to sock function */
    char    *ai_canonname;
    size_t  ai_addrlen;
    struct sockaddr *ai_addr; /* Pointer to socket address struct */
    struct addinfo  *ai_next; /* Pointer to next addrinfo struct */
}
~~~

The getaddinfo is given a host IP and service/port and some hints and constructs a linked list of addinfo strutcts, each pointing to a socket address struct that can be used with the socket interface. The client or server walks through the addrinfo struct trying each socket structure in *socket* and *connect* or *socket* and *bind* until the descriptor succceeds. The gai_strerror converst the error code returned by getaddinfo into actual string.

To pass the hints struct, memset is used to set the whole structure to zero, then only the ai_flags/family/stocktype/protocol can be set to fine tune the results; basically the first three arguments to the socket function filled in, along with some specific controls in flags. These include OR'd flags such as AI_PASSIVE which passes a *wildcard* host name for a listening socket that can work for multiple IP address of the server. In that case, the host argument in getaddrinfo must also be set to NULL.

### getnameinfo Function


This function uses the pointer *sa to a sockaddr to get the host and service. Lengths of each must be provided to the function along with optional flags that control for the format of the returned info.


~~~c
int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hotlen, char *service, size_t servlen, int flags);
~~~

What is missing from the book is the mention that the getaddrino and getnameinfo both get prior known information from somewhere over the nextwork, and fill out the details of the protocol family, socket types and protocols employed by the server, as well as available IPs given to the same hostname. The getnameinfo function can then simply walk down the linked list, converting the wanted info.

### Helper Functions

~~~c 
int open_clientfd (char *hostname, char *port);
int open_listenfd(char *port);
~~~

Either function opens a connection with a server or a client, meaning it returns a fully connected socket. It is nothing more than an initializer and a for loop for connecting to the client.

## The World Wide Web

Webservers either serve static content in the form of contents from a file stored on disk, or dynamic content by running a process and sending its output to the client. All files have a unique identifer known as a URL (universal resource locator, a tremendously ugly name). A '?' character serparates filename from its arguments which may be included with the URL, but how does all of this relate back to sockets and making connections? This entire discussion is based on HTTP (hypertext transfer protocol), a small protocol used in browsers and built on top of sockets. It's a higher level abstraction. An HTML program is written in the form of a page that is interpreted and executed by the browser after the content arrives.

The URL itself is the complete address of a file in question, built on top of the IP/hostname:

1.  The '\' expands into the default address like \index.html
2.  There are no set rules as to what is dynamic and what is static, but dynamic content is typically kept in the cgi-bin directory.



## Web Server

The HTTP standard specifies that every request must be followed by a carriage return and a newline ('\r\n'). HTTP requests consists of a request line followed by one or more request headers and an empty new line:

Method URI Version :: GET / HTTP/1.1
Header-name: header-data :: Host: www.aol.com

The response is a response line, followed by zero or more response headers and the content line itself. The reponse line consists of:

Version Status-code status-message

And the headers include the length of the conent as well as the type:

Content length: this-much
Content type: MIME-type(like text/html)

## CGI: Common Gateway Interface

Program name and arguments are passed as they are defined before: program-name?argument1&argument2. The CGI enviroment variable QUERY_STRING is set to argument1&argument2 which the called program can reference using getenv in the context of a child process, after execv. I am not sure why the program is not simply run with these arguments passed in execve, but there you go. Other information is passed on like the server port, request method, host address and content type/length (for POST requests only).

I am assuming that the CGI standard specifies that all output should be directed to the standard output, which is redirected using dup2 to the socket connecting the client.


## TINY

As a case study, just typing 192.168.8.1::5999/adder?1500&2300 into a browsergenerates the following in the page.

~~~html
404: Not found
Tiny couldn't find this file: ./home.html

The Tiny Web server
~~~


While on the "server" end, the following is put to the console:

~~~~bash
Accepted connection from (adhamkmopp, 59404)
GET / HTTP/1.1
Host: 192.168.8.101:5999
Connection: keep-alive
Cache-Control: max-age=0
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.77 Safari/537.36
DNT: 1
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9,es;q=0.8,ar;q=0.7
~~~~


Notably missing from the browser's end is the response line and response headers, but the server has indicated that it has received a GET request from the client. My assumption, not knowing anything about browsers, is that browsers translate URLs into request lines and sends them through a connection to somewhere. The rest of that are request headers sent by Chrome and printed by the request_hdr function in tiny, but continously checking for the return-carrriage-linefeed at the end of each header.
