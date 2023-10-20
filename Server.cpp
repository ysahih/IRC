#include "Server.hpp"
#include <sys/poll.h>


Server::Server() {
    memset(fds, 0, sizeof(fds));
}

Server::~Server() {close(this->_socketfd); }

void Server::setPort(short _port){
    this->_port = _port;
}

void Server::setSocket() {
    int tmp = 1;
    this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socketfd < 0)
        throw "Failed to create socket";
    if (setsockopt(this->_socketfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) < 0)
        throw "";
    if (fcntl(this->_socketfd, F_SETFL, O_NONBLOCK) < 0)
        throw "Failed to set socket to non-blocking mode";
}


void Server::setAddrInfo() {
    memset(&this->_addr, 0, sizeof(this->_addr));
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(this->_port);

	// this->_addr.sin_flags = AI_PASSIVE;
}

void Server::bindPort() {
    if (bind(this->_socketfd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0)
        throw "Failed to bind socket";
}


void Server::launch(){

    this->setSocket();
    this->setAddrInfo();
    this->bindPort();
   
    if (listen(this->_socketfd, 10) < 0)
        throw "Failed to listen for connections";
    std::cout << "Server listening on port: " << this->_port << std::endl;

    struct pollfd _pollfd;
    _pollfd.fd = this->_socketfd;
    _pollfd.events = POLLIN;
    _pollfd.revents = 0;
    int fdnbr = 0;
    this->fds[fdnbr++] = _pollfd;
    
    while (true) {
        if (poll(this->fds, 1, -1) < 0)
            throw "";
        for (int i = 1; i < fdnbr; i++){ /*through list of clients*/
                std::cout << "HEHE2\n";
            if (this->fds[i].fd != 0 &&  fds[i].revents == POLLIN){
                char buffer[1024];
                int bytesRead = recv(fds[i].fd, buffer, 1024, 0);
                std::cout << "Server ready to read" << std::endl;
                if (bytesRead <= 0){
                    if (errno == EWOULDBLOCK || errno == EAGAIN)
                        continue;
                    throw "Failed to receive data";
                }
                std::cout << "Received message: " << buffer << std::endl;
                const char* response = "Server: Message received\n";
                // int bytesSent = send(it->fd.fd, response, strlen(response), 0); //use of c functions
                // if (bytesSent < 0) 
                    throw "Failed to send response";
        
            }
        }
        std::cout << fdnbr<< std::endl;
        if (this->fds[0].fd != 0 &&  this->fds[0].revents == POLLIN){
            Client newClient;
            struct pollfd _clientpoll;
            // do {

            socklen_t len = sizeof(newClient._addr);
            newClient._sockfd = accept(this->_socketfd, (struct sockaddr*)&newClient._addr, &len);
            if (newClient._sockfd < 0){
                if (errno != EWOULDBLOCK)
                    throw "Failed to accept connection";   
                break;
            }
            _clientpoll.fd = newClient._sockfd;
            _clientpoll.events = POLLIN;
            _clientpoll.revents = 0;
            this->fds[fdnbr++] = _clientpoll;
            newClient.fd = _clientpoll;
            this->list.push_back(newClient);
            std::cout << "HEHE\n";
            // } while (true);
        }
        // else {
        //             std::cout << "HEHE1\n";
        // }
    }
    //    close(clientFd);
}

// }
