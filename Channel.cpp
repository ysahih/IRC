#include "Channel.hpp"

Channel::Channel(std::string name): _name(name){
	this->_size_limit = INT_MAX;
	this->_isPrivate = false;
	this->_isLimited = false;
	this->_isLocked = false;
	this->_isTopicRestricted = false;
}

void Channel::setOperator(int fd){this->_operators.push_back(fd);}

bool Channel::clientExist(Client A){return this->_list.find(A.getFd()) != this->_list.end();}

bool Channel::isOperator(int fd){
	return std::find(this->_operators.begin(), this->_operators.end(), fd) != this->_operators.end();
}

bool Channel::isTopicRestricted(){return this->_isTopicRestricted;}

void Channel::topicRestriction(bool switcher){this->_isTopicRestricted = switcher;}

void Channel::addClient(Client A){ this->_list[A.getFd()] = A;	}

void Channel::kickClient(std::string name){
	std::map<int, Client>::iterator it;
	for (it = this->_list.begin(); it != this->_list.end(); it++){
		if (it->second.getNick() == name){
			this->_list.erase(it);
			return;
		}
	}
}

void Channel::sendMessage(std::string msg, int fd){
	std::map<int, Client>::iterator it;
	for (it = this->_list.begin(); it != this->_list.end(); it++){
		if (it->first == fd)
			continue;
		int bytesSent = send(it->first , msg.c_str(), msg.length(), 0);
    	if (bytesSent < 0) 
        	throw std::runtime_error("Failed to send response");
    }
}

std::string Channel::getUsers(){
	std::string users = "";
	std::map<int, Client>::iterator it;
	for (it = this->_list.begin(); it != this->_list.end(); it++){
		if (it->first == this->getOwner())
			users += "@" + it->second.getNick() + " ";
		else if (this->isOperator(it->first))
			users += "+" + it->second.getNick() + " ";
		else
			users += it->second.getNick() + " ";
	}
	return users;
}

void Channel::setTopic(std::string topic){this->_topic = topic;}

bool Channel::isPrivate(){return this->_isPrivate;}

void Channel::setPassword(std::string pass){ this->_password = pass; }

void Channel::setSizeLimit(int size){this->_size_limit = size;}

int Channel::getSizeLimit(){return this->_size_limit;}

int Channel::numberOfClients(){return this->_list.size();}


bool Channel::isFull(){return this->_list.size() >= this->_size_limit;}

bool Channel::isLimited(){return this->_isLimited ;}

std::string Channel::getTopic(){return this->_topic;}

std::string Channel::getPassword(){return this->_password;}

bool Channel::isLocked(){return this->_isLocked;}

void Channel::setPrivate(bool switcher){this->_isPrivate = switcher;}

void Channel::setLocked(bool switcher){this->_isLocked = switcher;}

void Channel::setLimited(bool switcher){this->_isLimited = switcher;}

int Channel::getOwner(){return this->_operators[0];}

void Channel::kickOperator(int fd){
	std::vector<int>::iterator it;
	for (it = this->_operators.begin(); it != this->_operators.end(); it++){
		if (*it == fd){
			this->_operators.erase(it);
			return;
		}
	}
}
