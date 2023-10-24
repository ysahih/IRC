#include "Channel.hpp"

Channel::Channel(std::string name): _name(name), _isPrivate(0){
	this->_size_limit = INT_MAX;
	this->_isLimited = false;
	this->_isLocked = false;
}

void Channel::setOperator(int fd){this->_operators.push_back(fd);}

bool Channel::clientExist(Client A){return this->_list.find(A.getFd()) != this->_list.end();}

bool Channel::isOperator(int fd){
	return std::find(this->_operators.begin(), this->_operators.end(), fd) != this->_operators.end();
}

void Channel::addClient(Client A){ this->_list[A.getFd()] = A;	}

void Channel::kickClient(std::string name){
	std::map<int, Client>::iterator it;
	for (it = this->_list.begin(); it != this->_list.end(); it++){
		if (it->second.getNick() == name){
			this->_list.erase(it);
			return;
		}
	}
	throw "client not found";
}

void Channel::sendMessage(std::string msg){
	std::map<int, Client>::iterator it;
	for (it = this->_list.begin(); it != this->_list.end(); it++){
		int bytesSent = send(it->first , msg.c_str(), msg.length(), 0);
    	if (bytesSent < 0) 
        	throw "Failed to send response";
    }
}

void Channel::setTopic(std::string topic){this->_topic = topic;}

bool Channel::isPrivate(){return this->_isPrivate;}

void Channel::setPassword(std::string pass){ this->_password = pass; }

void Channel::setSizeLimit(int size){this->_size_limit = size;}

int Channel::getSizeLimit(){return this->_size_limit;}

int Channel::numberOfClients(){return this->_list.size();}

bool Channel::isFull(){return this->_list.size() == this->_size_limit;}

bool Channel::isLimited(){return this->_isLimited ;}

std::string Channel::getPassword(){return this->_password;}

bool Channel::isLocked(){return this->_isLocked;}