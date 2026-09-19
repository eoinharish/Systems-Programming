#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> // const struct sockaddr_in
#include <cstdint> // uint16_t
#include <cstring> //strerror

// socket (domain, socket_type, protocol) 
int CreateServerSocket()
{
    // Create IPv4 TCP socket.
    // Kernel returns a socket file descriptor.
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1)
    {
        std::cerr << "socket() failed\n";
        return -1;
    }

    std::cout << "Socket FD: " << socket_fd << '\n';

    return socket_fd;
}

// bind (fd, sockaddr_in, sizeof(sockaddr_in))
//
// sockaddr_in (conceptually)
// ┌────────────────────────────┐
// │ sin_family                 │ → AF_INET
// │ sin_port                   │ → 8080
// │ sin_addr                   │ → 127.0.0.1
// └────────────────────────────┘
bool BindServerSocket(int fd, uint16_t port)
{
    sockaddr_in server_addr{};

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); // host to network short (host-byte order to network-byte order)
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // host to network long (IPv4 is 32-bit (4 bytes,i,e, long))

    if (bind(fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
    {
        std::cerr << "bind() failed: " << std::strerror(errno) << '\n';
        return false;
    }

    std::cout << "Socket bound to 127.0.0.1:" << port << '\n';
    return true;
}

// listen (int socket_fd, int backlog)
// backlog: maximum number of pending connections waiting to be accepted
bool ListenForConnections(int fd, int backlog)
{
    if (listen(fd, backlog) == -1)
    {
        std::cerr << "listen() failed\n";
        return false;
    }

    std::cout << "Server is listening...\n";
    return true;
}

// accept(server_fd) removes one pending client connection from the listening queue and 
// returns a client's socket FD for communicating with that client.
//
// IMP: By default, accept() is BLOCKING.
//      If no client is waiting, the server pauses at: accept(server_fd, ...) until a client connects.
//      Use netcat linux cmd: nc 127.0.0.1 8080 to test your server socket
int AcceptClient(int server_fd)
{
    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);

    std::cout << "Calling accept()\n";
    int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);
    std::cout << "accept() finished\n";

    if (client_fd == -1)
    {
        std::cerr << "accept() failed\n";
        return -1;
    }

    std::cout << "Client Connected. Client FD: " << client_fd << '\n';

    return client_fd;
}

int main()
{
    ////////////////////////////////////
    int server_fd = CreateServerSocket();

    if (server_fd == -1)
    {
        std::cerr << "socket() failed" << "\n";
        return 1;
    }

    constexpr uint16_t port = 8080; // TCP/UDP port is 2 bytes

    // Assign IP address and port to the socket
    //////////////////////////////////////////
    if (!BindServerSocket(server_fd, port))
    {
        close(server_fd);
        return 1;
    }

    // Allow socket to listen to connections
    /////////////////////////////////////////////////
    if (!ListenForConnections(server_fd, SOMAXCONN))
    {
        close(server_fd);
        return 1;
    }

    // accept() is a BLOCKING call (blocks until the client connects)
    /////////////////////////////////////////
    int client_fd = AcceptClient(server_fd);
    if (client_fd == -1)
    {
        close(server_fd);
        return 1;
    }

    // recv() - send() - repeat
    char buffer[4096];
    while (true)
    {
        // recv() is a BLOCKING call (blocks until receives data from client)
        // ssize_t -> signed size_t
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
        
        std::cout << "bytes_received: " << bytes_received << '\n';

        if (bytes_received == -1)
        {
            perror("recv() failed");
            break;
        }

        if (bytes_received == 0)
        {
            std::cout << "Client disconnected\n";
            break;
        }

        ssize_t bytes_sent = send(client_fd, buffer, bytes_received, 0);
        
        std::cout << "bytes_sent: " << bytes_sent << '\n';

        if (bytes_sent == -1)
        {
            perror("send() failed");
            break;
        }

    }


    close(client_fd);
    close(server_fd);
    //sleep(60); // sleep for seconds
    return 0;
}