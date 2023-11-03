#include "Client.hpp"
#include <sys/poll.h>

Client::Client(): _authenticate(0), _welcomed(0), _registered(0) {}



void Client::authenticate(){
    if (this->_authenticate)
        return ;
    if (!this->_nickname.empty() && !this->_username.empty()){
        this->_authenticate = true;
    }
}

void Client::initClient(struct pollfd _poll, struct sockaddr_in _addr){
    char *str;
    this->_sockfd = _poll.fd;
    this->fd = _poll;
    this->_addr = _addr;
    str = inet_ntoa(_addr.sin_addr);
    this->IPAddr = str;
}

struct sockaddr_in Client::getAddr(){return this->_addr;}

bool Client::isRegistered(){return this->_registered;}
std::string Client::getIP(){return this->IPAddr;} 

std::string Client::joinBuffer(std::string msg){
    this->_buffer += msg;
    return this->_buffer;    
}
bool Client::isBuffered(){
    return this->_buffer.empty() == false;
}

void Client::clearBuffer(){
    this->_buffer.clear();
}

void Client::setRegistered(bool registered){this->_registered = registered;}

void Client::setNick(std::string name){this->_nickname = name;}
void Client::setUser(std::string name){this->_username = name;}

void Client::setWelcomed(bool welcomed){this->_welcomed = welcomed;}

bool Client::isAuthenticate(){return this->_authenticate;}
bool Client::isWelcomed(){return this->_welcomed;}
std::string Client::getNick(){return this->_nickname;}
std::string Client::getUser(){return this->_username;}
int Client::getFd(){return this->_sockfd;}