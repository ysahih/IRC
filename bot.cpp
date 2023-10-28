#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int BUFFER_SIZE = 4096;

void send_message(int socket, const std::string& message) {
    std::string formatted_message = message + "\n";
    send(socket, formatted_message.c_str(), formatted_message.length(), 0);
    usleep(500);
}

int main() {
    // Configuration
    const std::string server = "localhost";
    const int port = 8080;
    const std::string nickname = "SYKONO";
    const std::string channel = "#Gaming";

    // Create socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        std::cout << "Failed to create socket." << std::endl;
        return 1;
    }

    // Connect to the server
    sockaddr_in server_addr; 
    if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) <= 0)
    {
        std::cout << "Invalid address/Address not supported" << std::endl;
        return 1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Failed to connect to the server." << std::endl;
        return 1;
    }

    // Send user and nickname information
    send_message(socket_desc, "PASS hello");
    send_message(socket_desc, "NICK " + nickname);
    send_message(socket_desc, "USER " + nickname + " 0 * :" + nickname);

    // Join the channel
    send_message(socket_desc, "JOIN " + channel);

    // Main loop
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        if (recv(socket_desc, buffer, sizeof(buffer), 0) <= 0) {
            std::cout << "Connection closed by the server." << std::endl;
            break;
        }
        std::string message(buffer);
        std::cout << message;
    }
    close (socket_desc);
    return 0;
}