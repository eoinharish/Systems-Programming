#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // inet_pton()


bool ConnectToServer(int client_fd, const char* server_ip, uint16_t server_port)
{
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    // Convert textual IPv4 to binary form
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) != 1)
    {
        std::cerr << "Invalid server IP address\n";
        return false;
    }
    
    // connect(client_fd, &server_addr, sizeof(server_addr))
    if (connect(client_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
    {
        std::cerr << "connect() failed\n";
        return false;
    }

    std::cout << "Client Connected to Server\n";
    return true;
}

int main()
{
    constexpr uint16_t server_port = 8080;

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd == -1)
    {
        std::cerr << "socket() failed\n";
        return 1;
    }

    if (!ConnectToServer(client_fd, "127.0.0.1", server_port))
    {
        close(client_fd);
        return 1;
    }

    const char message[] = "hello\n";
    char buffer[1024];

    constexpr size_t iterations = 1;

    for(size_t i{0}; i < iterations; ++i)
    {
        send(client_fd, message, sizeof(message) - 1, 0);
        recv(client_fd, buffer, sizeof(buffer), 0);
    }

    close(client_fd);
    return 0;
}