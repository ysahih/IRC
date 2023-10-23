#include "Channel.hpp"

Channel::Channel(std::string name): _name(name){}

void Channel::setOperator(int fd){this->operator_fd = fd;}

bool Channel::clientExist(Client A){return this->_list.find(A.getFd()) != this->_list.end();}

void Channel::addClient(Client A){ this->_list[A.getFd()] = A; }

void Channel::kickClient(std::string name){
	std::map<int, Client>::iterator it;
	for (it = this->_list.begin(); it != this->_list.end(); it++){
		if (it->second.getNick() == name){
			this->_list.erase(it);
			return;
		}
	}
	// throw "client not found";
}

void Channel::sendMessage(std::string msg){
	std::map<int, Client>::iterator it;
	for (it = this->_list.begin(); it != this->_list.end(); it++){
		int bytesSent = send(it->first , msg.c_str(), msg.length(), 0); //!!use of c functions
    	if (bytesSent < 0) 
        	throw "Failed to send response";
    }
}