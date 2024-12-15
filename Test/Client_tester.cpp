#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

void simulateClient(const char *server_ip, int server_port) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Client setup
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Client: Socket creation error" << std::endl;
        return;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Client: Invalid address/ Address not supported" << std::endl;
        return;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Client: Connection Failed" << std::endl;
        return;
    }

    // Send message to server
    const char* ulysse = "Heureux qui comme Ulysse";
    send(sock, ulysse, strlen(ulysse), 0);
    std::cout << "Client: message sent" << std::endl;

    // Read server response
    read(sock, buffer, 1024);
    std::cout << "Client: Server response: " << buffer << std::endl;

    const char* voyage = "A fait un beau voyage";
    send(sock, voyage, strlen(voyage), 0);
    std::cout << "Client: message sent" << std::endl;

    read(sock, buffer, 1024);
    std::cout << "Client: Server response: " << buffer << std::endl;

    // close connexion
    close(sock);
    std::cout << "Client: Disconnected" << std::endl;
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>" << std::endl;
        return 1;
    }

    const char* server_ip = argv[1];
    int server_port = std::atoi(argv[2]);

    simulateClient(server_ip, server_port);

    return 0;
}