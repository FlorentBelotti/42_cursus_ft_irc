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

    // Send nickname command
    std::cout << "\n--- TRUE NICKNAME ---" << std::endl;
    const char* nickTrue = "/nickname test";
    send(sock, nickTrue, strlen(nickTrue), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));

    std::cout << "\n--- FALSE NICKNAME ---" << std::endl;
    const char* nickFalse = "/nickname te**st";
    send(sock, nickFalse, strlen(nickFalse), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));

    // Send user command
    std::cout << "\n--- TRUE USER ---" << std::endl;
    const char* userTrue = "/user testuser testhost testserver testname";
    send(sock, userTrue, strlen(userTrue), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));

    std::cout << "\n--- FALSE USER ---" << std::endl;
    const char* userFalse = "/user testuser testhost testserver";
    send(sock, userFalse, strlen(userFalse), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));

    // Send join command
    std::cout << "\n--- TRUE JOIN ---" << std::endl;
    const char* joinTrue = "/join #general";
    send(sock, joinTrue, strlen(joinTrue), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));

    std::cout << "\n--- FALSE JOIN ---" << std::endl;
    const char* joinFalse = "/join #toolongchannelname";
    send(sock, joinFalse, strlen(joinFalse), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));

    std::cout << "\n--- TRUE PRVMSG ---" << std::endl;
    const char* prvmsgTrue = "/privmsg #general Ceci est un test";
    send(sock, prvmsgTrue, strlen(prvmsgTrue), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));

    std::cout << "\n--- FALSE PRVMSG ---" << std::endl;
    const char* prvmsgFalse = "/privmsg #test Ceci est un test";
    send(sock, prvmsgFalse, strlen(prvmsgFalse), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));

    std::cout << "\n--- FALSE PART ---" << std::endl;
    const char* partfalse = "/part #falsechannelname";
    send(sock, partfalse, strlen(partfalse), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));

    std::cout << "\n--- TRUE PART ---" << std::endl;
    const char* truepart = "/part #general";
    send(sock, truepart, strlen(truepart), 0);
    read(sock, buffer, 1024);
    std::cout << "Server response: " << buffer << std::endl;
    memset(buffer, 0, sizeof(buffer));

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
    int server_port = std::atoi(argv[2]); // Utilisez std::atoi

    simulateClient(server_ip, server_port);

    return 0;
}