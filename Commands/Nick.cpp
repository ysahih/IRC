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
	it->second.setNick(name);
	it->second.authenticate();
	if (it->second.isWelcomed() == false && it->second.isAuthenticate() == true){
		this->sendMessage(fd, ":" + this->_hostname + " 001  " + name + " :Welcome to the IRC Network, " + name + "!\r\n");
		it->second.setWelcomed(true);
	}
}