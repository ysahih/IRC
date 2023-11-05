#include "Server.hpp"
#include <sys/poll.h>



Server::Server(std::string port, std::string password) {
    memset(_fds, 0, sizeof(_fds));
    this->_port = atoi(port.c_str());
    if (this->_port <= 0)
        throw std::runtime_error("Invalid port");
    this->_password = password;
    if (this->_password.empty())
        throw std::runtime_error("Invalid password");
}

void Server::sendToAll(std::string msg){
    for (std::map<int, Client>::iterator it = this->list.begin(); it != this->list.end(); it++){
        if (it->second.isRegistered() == true)
            this->sendMessage(it->first, msg);
    }
}

Server::~Server() {
    std::map<int, Client>::iterator it = this->list.begin();
    for (; it != this->list.end(); it++){
        close(it->first);
    }
    close(this->_socketfd);
}


void Server::setSocket() {
    int tmp = 1;
    this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socketfd < 0)
        throw std::runtime_error("Failed to create socket");
    if (setsockopt(this->_socketfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) < 0)
        throw std::runtime_error("Failed to set socket reuse option");
    if (fcntl(this->_socketfd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Failed to set socket to non-blocking mode");
}

std::string Server::getHostName()
{
    std::system("hostname > ip.txt");
    std::ifstream ip("ip.txt");
    std::string hostname;
    std::getline(ip, hostname);
    std::remove("ip.txt");
    return (hostname);
}

void Server::setAddrInfo() {
    memset(&this->_addr, 0, sizeof(this->_addr));
    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(this->_port);
    this->_hostname = this->getHostName();
    struct pollfd _pollfd;
    _pollfd.fd = this->_socketfd;
    _pollfd.events = POLLIN;
    _pollfd.revents = 0;
    this->_fds[0] = _pollfd;
}

void Server::bindPort() {
    if (bind(this->_socketfd, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) < 0)
        throw std::runtime_error("Failed to bind socket");
}


std::string Server::addClient(struct pollfd _poll, std::string line)
{
    std::stringstream iss(line);
    std::string pass;
    iss >> pass;
    if (pass != "PASS")
        return "464 :Password incorrect\r\n";
    iss >> pass;
    if (!pass.empty() && pass[0] == ':')
        pass = pass.substr(1);
    if (pass != this->_password)
        return "464 :Password incorrect\r\n";
    this->list[_poll.fd].setRegistered(true);
    return "";
}

void Server::launch() {

    this->setSocket();
    this->setAddrInfo();
    this->bindPort();
   
    if (listen(this->_socketfd, 10) < 0)
        throw std::runtime_error("Failed to listen for connections");

  
    int fdnbr = 1;
    while (true) {
        if (poll(this->_fds, SIZE, -1) < 0)
            throw std::runtime_error("poll failed");

        for (int i = 1; i < fdnbr; i++){ /*through list of clients*/
            if (this->_fds[i].revents & POLLIN) {
                char buffer[1024];
                int bytesRead = recv(_fds[i].fd, buffer, 1024, 0);
                if (bytesRead < 0)
                    throw std::runtime_error("Failed to receive data");
                if (bytesRead == 0)
                    this->quit(this->_fds[i].fd);
                std::string line;
                line.assign(buffer, bytesRead);
                if (line[line.length() - 1] != '\n'){
                    this->list[this->_fds[i].fd].joinBuffer(line);
                    continue;
                }
                else{
                    line = this->list[this->_fds[i].fd].joinBuffer(line);
                    this->list[this->_fds[i].fd].clearBuffer();
                }

                std::stringstream iss(line);
                while (std::getline(iss, line, '\n')){
                    if (this->list[this->_fds[i].fd].isRegistered() == false){
                        std::string message = this->addClient(this->_fds[i], line);
                        if (!message.empty())
                            this->sendMessage(this->_fds[i].fd, message);
                    }
                    else
                        this->parse(this->_fds[i].fd, line);
                }
            }
        }
        if (this->_fds[0].fd != 0 &&  this->_fds[0].revents == POLLIN) { // if the event is on the socket fd; we add the new_fd to the list.
           
            struct sockaddr_in addr;
            struct pollfd _clientpoll;
            socklen_t len = sizeof(addr);
            Client newClient;
            _clientpoll.fd  = accept(this->_socketfd, (struct sockaddr *)&addr, &len);
            if (_clientpoll.fd < 0)
                throw std::runtime_error("Failed to accept connection");
            _clientpoll.events = POLLIN;
            _clientpoll.revents = 0;
            newClient.initClient(_clientpoll, addr);
            this->_fds[fdnbr++] = _clientpoll;
            this->list[_clientpoll.fd] = newClient;
        }
    }
}
