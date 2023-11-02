#include "../Server.hpp"




void Server::setNick(int fd, std::stringstream& iss) {
	
	std::string name;
	std::string tmp;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;

/*(461)

  "<client> <command> :Not enough parameters"
*/
	if (iss >> tmp){
		this->sendMessage(fd, "461 " + name + " NICK :Invalid parameteres\r\n");
		return ;
	}
	if (name.empty() || name == ":"){
		this->sendMessage(fd, "461 " + name + " NICK :Not enough parameters\r\n");
		return ;
	}
	if (this->findClient(name) != -1){
		this->sendMessage(fd, "433 " + name + " :Nickname is already in use\r\n");
		return ;
	}
	std::string oldName = it->second.getNick();
	it->second.setNick(name);
	it->second.authenticate();
	if (it->second.isWelcomed() == false && it->second.isAuthenticate() == true){
		// this->sendMessage(fd, ":" + this->_hostname + "   " + name + " :Welcome to the IRC Network, " + name + "!\r\n");
		this->sendMessage(fd, ":" + this->_hostname + " 001 " + this->list[fd].getNick() + " :Welcome " + this->list[fd].getNick() + " to the IRC Network !\r\n");
		this->sendMessage(fd, ":" + this->_hostname + " 002 " + this->list[fd].getNick() + " :Your host is " + this->_hostname + "running version 10.1.1 !\r\n");
		this->sendMessage(fd, ":" + this->_hostname + " 003 " + this->list[fd].getNick() + " :This server was created 2023-10-15 !\r\n");
		this->sendMessage(fd, ":" + this->_hostname + " 004 " + this->list[fd].getNick() + " :Host: " + this->_hostname + ", Version: 1.0, User mode: none, Channel mode: o, i, l, k, t !\r\n");
		it->second.setWelcomed(true);
		return ;
	}

	this->sendToAll(":" + oldName + "!~" + it->second.getUser() + "@localhost" + " NICK :" + name + "\r\n");

	

}