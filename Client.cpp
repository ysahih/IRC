#include <cstring>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to connect to the server" << std::endl;
        return 1;
    }

    std::cout << "Connected to the server" << std::endl;

    // Send a message to the server
    const char* message = "Hello from the client";
    int bytesSent = send(clientSocket, message, strlen(message), 0);
    if (bytesSent < 0) {
        std::cerr << "Failed to send data" << std::endl;
        return 1;
    }

    // Receive a response from the server
    int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesRead < 0) {
        std::cerr << "Failed to receive data" << std::endl;
        return 1;
    }

    // Print the response received from the server
    std::cout << "Response from server: " << buffer << std::endl;

    // Close the socket
    close(clientSocket);

    return 0;
}