#include "Client.hpp"
#include <sys/poll.h>

Client::Client(): _authenticate(0){}

void Client::authenticate(){
    
    if (!this->_nickname.empty() && !this->_username.empty())
        this->_authenticate = true;
}

void Client::initClient(struct pollfd _poll){
    this->_sockfd = _poll.fd;
    this->fd = _poll;
}

void Client::setNick(std::string name){this->_nickname = name;}
void Client::setUser(std::string name){this->_username = name;}


bool Client::is_authenticate(){return this->_authenticate;}

std::string Client::getNick(){return this->_nickname;}
std::string Client::getUser(){return this->_username;}
int Client::getFd(){return this->_sockfd;}