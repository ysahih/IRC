#include "Server.hpp"
#include <sys/poll.h>


Server::Server() {
    memset(_fds, 0, sizeof(_fds));
    this->_password = "hello";
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

    struct pollfd _pollfd;
    _pollfd.fd = this->_socketfd;
    _pollfd.events = POLLIN;
    _pollfd.revents = 0;
    this->_fds[0] = _pollfd;
}

void Server::bindPort() {
    if (bind(this->_socketfd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0)
        throw "Failed to bind socket";
}


bool Server::addClient(struct pollfd _poll)
{
    Client newClient;
    newClient.initClient(_poll);

    char buffer[1024];
    int bytesRead = recv(_poll.fd, buffer, 1024, 0);
    buffer[bytesRead] = 0x0;
    if (bytesRead <= 0){
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return 0;
        throw "Failed to receive data";
    }
     std::string line;
    line.assign(buffer, bytesRead);
    std::stringstream iss(line);
    std::string pass;
    iss >> pass;
    if (pass != "PASS")
        return 0;
    iss >> pass;
    if (pass != this->_password)
        return 0;
    this->list[_poll.fd] = newClient;
    const char* response = "Client added without authentication\n";
    int bytesSent = send(_poll.fd, response, strlen(response), 0); //!use of c functions
    if (bytesSent < 0) 
        throw "Failed to send response";
    return 1;
}

void Server::launch(){

    this->setSocket();
    this->setAddrInfo();
    this->bindPort();
   
    if (listen(this->_socketfd, 10) < 0)
        throw "Failed to listen for connections";

  
    int fdnbr = 1;
    
    std::string a; 
    a.find_first_not_of(' ');
    while (true) {
        if (poll(this->_fds, 1024, -1) < 0)
            throw "";
        for (int i = 1; i < fdnbr; i++){ /*through list of clients*/
            if (this->_fds[i].revents == POLLIN) { // check if there is an event on one of our clients.
               
                //if the client has entered the PASS , they would be added to the list
                // otherwise compare the password and add them/;
                std::map<int, Client>::iterator it = this->list.find(this->_fds[i].fd);
                if (it == list.end()){
                    if (this->addClient(this->_fds[i]) == 0)
                        this->sendMessage(this->_fds[i].fd, "Error(): PASS <password>\n"); // send a message with the cmds form
                }
                else { // here the client is already within the server;
                    char buffer[1024];
                    int bytesRead = recv(_fds[i].fd, buffer, 1024, 0);
                    
                    if (bytesRead <= 0){
                        if (errno == EWOULDBLOCK || errno == EAGAIN)
                            continue;
                        throw "Failed to receive data";
                    }
                    std::string line;
                    line.assign(buffer, bytesRead);
                    // buffer[bytesRead] = 0x0;
                    try{
                        this->parse(this->_fds[i].fd, line);
                    }catch(const char *s){this->sendMessage(this->_fds[i].fd, s);}
                }
        
            }
        }
        if (this->_fds[0].fd != 0 &&  this->_fds[0].revents == POLLIN) { // if the event is on the socket fd; we add the new_fd to the list.
           
            struct pollfd _clientpoll;
            _clientpoll.fd  = accept(this->_socketfd, NULL, NULL);
            if (_clientpoll.fd < 0){
                if (errno != EWOULDBLOCK)
                    throw "Failed to accept connection";
                break;
            }
            // save the file discriptor 
            _clientpoll.events = POLLIN;
            _clientpoll.revents = 0;
            this->_fds[fdnbr++] = _clientpoll;\
        }
    }
}
