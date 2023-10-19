#include "Server.hpp"


Server::Server() {}

Server::~Server() {close(this->_socketfd); }

void Server::setPort(short _port){
    this->_port = _port;
}

void Server::setSocket() {
    this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socketfd < 0)
        throw "Failed to create socket";
}


void Server::setAddrInfo() {
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(this->_port);
	// this->_addr.ai_flags = AI_PASSIVE;
}

void Server::bindPort() {
    if (bind(this->_socketfd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0)
        throw "Failed to bind socket";
}


void Server::launch(){

    this->setSocket();
    this->setAddrInfo();
    this->bindPort();
    while (true){
        if (listen(this->_socketfd, 10) < 0)
            throw"Failed to listen for connections";
        std::cout << "Server listening on port: " << this->_port << std::endl;

        struct sockaddr_in clientAddr;
        socklen_t len = sizeof(clientAddr);
        int clientFd = accept(this->_socketfd, (struct sockaddr*)&clientAddr, &len);
        if (clientFd < 0) 
            throw "Failed to accept connection";

        char buffer[1024];
        int bytesRead = recv(clientFd, buffer, 1024, 0);
        if (bytesRead < 0) 
            throw "Failed to receive data";
        std::cout << "Received message: " << buffer << std::endl;


        const char* response = "Server: Message received";
        int bytesSent = send(clientFd, response, strlen(response), 0);
        if (bytesSent < 0) 
            throw "Failed to send response";

        close(clientFd);
    }

}





