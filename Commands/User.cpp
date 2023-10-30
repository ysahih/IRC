#include "../Server.hpp"



void Server::setUser(int fd, std::stringstream& iss) {

	std::string name;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;


	if (name.empty() || name == ":"){
		this->sendMessage(fd, "461 " + name + " USER :Not enough parameters\r\n");
		return ;
	}
	it->second.setUser(name);
	it->second.authenticate();
	if (it->second.isWelcomed() == false && it->second.isAuthenticate() == true){
		this->sendMessage(fd, ":" + this->_hostname + " 001  " + name + " :Welcome to the IRC Network, " + name + "!\r\n");
		it->second.setWelcomed(true);
	}
}
