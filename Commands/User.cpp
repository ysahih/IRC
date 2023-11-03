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
    	std::string msg = ":" + this->_hostname + " 001 " +  this->list[fd].getNick() +  " :Welcome to the Internet Relay Network " + this->list[fd].getNick() + "!~" + this->list[fd].getUser() + "@" + "127.0.0.1\r\n";
    	msg += ":" + _hostname + " 002 " +  this->list[fd].getNick() + " :Your host is " + _hostname + ", running version leet-irc 1.0.0\r\n";
    	msg += ":" + _hostname + " 003 " +  this->list[fd].getNick() + " :This server has been started Wed Oct 12 2022\r\n";
    	msg += ":" + _hostname + " 004 " +  this->list[fd].getNick() + " " + _hostname + " leet-irc 1.0.0 aioOrsw aovimntklbeI\r\n";
    	msg += ":" + _hostname + " 251 " + this->list[fd].getNick() + " :There are " +  std::to_string(list.size()) + " users and 1 bot on 1 servers\r\n";
    	msg += ":" + _hostname + " 375 " + this->list[fd].getNick() + " :- " + _hostname + " Message of the day -\r\n";
    	msg += ":" + _hostname + " 376 " + this->list[fd].getNick() + " :End of MOTD command\r\n";
		this->sendMessage(fd, msg);
		it->second.setWelcomed(true);
	}
	
}
