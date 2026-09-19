# TCP Echo Server / Client

- strace ./program
    Linux tool used to trace the system calls made by the process.

- getpid() is libc interface to obtain PID. It may or may not require an actual kernel transition depending on the implementation.

#libc: C standard library; provides APIs/wrappers such as malloc, read, write, getpid.

-----------------------------------------------------------------
## 1. socket()

socket() is a system call that asks the kernel to create a socket endpoint and returns a file descriptor to the socket.
- File descriptor (FD) table:
    Per-process table mapping integers like 0, 1, 2, 3 to kernel-managed I/O resources (eg. files, terminals, pipes and sockets)
    Created by the kernel (in kernel-space) and can be inspected using:
    ls -l /proc/<PID>/fd : Shows FD table (while the program is running)

- cat /proc/<PID>/maps: Shows broad memory mapped regions - [executable, heap, mmap regions (.so, anonymous mem), stack]


### API
socket(domain, type, protocol)

domain:
    AF_INET   → IPv4
    AF_INET6  → IPv6
    AF_UNIX   → Local IPC

type:
    SOCK_STREAM → byte stream → TCP
    SOCK_DGRAM  → datagrams → UDP
    SOCK_RAW    → raw packets

protocol:
    0           → default protocol
    IPPROTO_TCP → TCP
    IPPROTO_UDP → UDP

Return:
    >= 0 → socket FD
    -1   → error

-----------------------------------------------------------------
## 2. bind()

### Purpose

Assigns a local IP address and port to a socket.

### API

int bind(int sockfd,
         const struct sockaddr *addr,
         socklen_t addrlen);

socket_fd:
        Socket file descriptor
addr:
        Local address and port (struct sockaddr_in)
addrlen:
        Size of address structure (sizeof(socket_addr))

Return:
        0   → success
        -1  → error (errno contains reason)

### IPv4 address structure

sockaddr_in:
    sin_family        → Address family (AF_INET)
    sin_port          → Port number
    sin_addr.s_addr   → IPv4 address

### Example

sockaddr_in server_addr{};

server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(8080);
server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

bind(socket_fd,
     reinterpret_cast<sockaddr*>(&server_addr),
     sizeof(server_addr));

### Mental model

socket()
    ↓
Create socket
    ↓
bind()
    ↓
Assign local IP + port

### Important

bind() does NOT establish a TCP connection.
bind() does NOT start accepting connections.
listen() is responsible for putting a socket into listening mode.

### Byte Order — Quick Notes

Consider the 16-bit hexadecimal number: 0x1234
It contains 2 bytes: 0x12 0x34

Big-endian:    Most significant byte is stored first (0x1234)
               Address A:   0x12
               Address A+1: 0x34
Little-endian: Least significant byte is stored first (0x3412)
               Address A:   0x34
               Address A+1: 0x12

Network byte order: Standard byte order used by networking protocols; it is "Big-endian".

-----------------------------------------------------------------
## 3. listen()

### Purpose

Converts a bound TCP socket into a PASSIVE listening socket.

It allows the server to wait for incoming connection requests.

### API

int listen(int socket_fd, int backlog);

socket_fd: 
        Server socket file descriptor
backlog:
        Maximum number of pending connections waiting to be accepted

Return:
        0   → success
        -1  → error (errno contains reason)

### Example

if (listen(server_fd, SOMAXCONN) == -1)
{
    std::cerr << "listen() failed\n";
}

### Socket lifecycle

socket()
    ↓
Create socket
    ↓
bind()
    ↓
Assign local IP + port
    ↓
listen()
    ↓
Mark socket as passive/listening

### Important distinctions

socket()  → Creates a socket endpoint
bind()    → Assigns a local IP address and port
listen()  → Enables the socket to receive connection requests

### Mental model

The listening socket is like a reception desk.

Clients arrive and wait in a queue.
accept() takes one waiting client from that queue and creates a connected socket for communication.

-----------------------------------------------------------------
## 4. accept()

### Purpose

Accepts an incoming client connection from the listening queue.

Returns a new socket FD (client's fd) for communicating with that client.

int accept(int sockfd,
           struct sockaddr *addr,
           socklen_t *addrlen);

### Key distinction

server_fd → Listening socket; accepts new connections
client_fd → Connected socket; communicates with one client

### Behavior

- Blocking by default if no client is waiting.
- The listening socket remains open for additional clients.
- Returns -1 on failure.

### Example

int client_fd = accept(server_fd, ...);

### Socket relationship

server_fd
    ↓
listen()
    ↓
Pending client connections
    ↓
accept()
    ↓
client_fd

---

## 5. nc (Netcat)

### Purpose

Netcat is a command-line networking utility that can act as a basic TCP or UDP client/server.

It is useful for testing socket programs. nc 127.0.0.1 8080

### Example

Terminal 1                 Terminal 2
-----------                -----------
./server                   nc 127.0.0.1 8080
    │                              │
    └──── accept() ◄───────────────┘

This connects to TCP port 8080 on the local machine.

### In our project

Netcat acts as a temporary TCP client to test whether our server:

- Is listening on the expected port.
- Accepts incoming connections.
- Creates a connected client socket using accept().


## TCP 3-Way Handshake and Data Transfer Between Client - Server

 - Setup: Server is listening on 127.0.0.1:8080. Client connects to the Server and sends "hello\n" to server, and server sends the same message
       back to the client.

 - connect() triggers the TCP three-way handshake.

---------------------------------------------------------
sudo tcpdump -i lo -nn -S 'tcp port 8080'
---------------------------------------------------------            
tcpdump: verbose output suppressed, use -v[v]... for full protocol decode
listening on lo, link-type EN10MB (Ethernet), snapshot length 262144 bytes

10:17:07.833819 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [S], seq 3813067815, length 0
10:17:07.833865 IP 127.0.0.1.8080 > 127.0.0.1.49216: Flags [S.], seq 1854576707, ack 3813067816, length 0
10:17:07.833905 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [.], ack 1854576708, length 0

10:17:07.834073 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [P.], seq 3813067816:3813067822, ack 1854576708, length 6: HTTP
10:17:07.834106 IP 127.0.0.1.8080 > 127.0.0.1.49216: Flags [.], ack 3813067822, length 0
10:17:07.834319 IP 127.0.0.1.8080 > 127.0.0.1.49216: Flags [P.], seq 1854576708:1854576714, ack 3813067822, length 6: HTTP
10:17:07.834349 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [.], ack 1854576714, length 0

10:17:07.834501 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [F.], seq 3813067822, ack 1854576714, length 0
10:17:07.834660 IP 127.0.0.1.8080 > 127.0.0.1.49216: Flags [F.], seq 1854576714, ack 3813067823, length 0
10:17:07.834694 IP 127.0.0.1.49216 > 127.0.0.1.8080: Flags [.], ack 1854576715, length 0

## 3-way handshake

Client->Server [S]  (SYN)       -> to initiate the connection (seq = x)
Server->Client [S.] (SYN + ACK) -> acknowleges and respond (ack = x+1, seq = y)
Client->Server [.]  (ACK)       -> acknowledge (ack = y+1)

[P.] (PSH + ACK) - data transmission
[F.] (FIN + ACK) - gracefully close connection
[R]  (RST - reset/abort connection)

Sequence number: Number of the first byte in a TCP segment. 
Acknowledgment number: Next byte the receiver expects to receive.