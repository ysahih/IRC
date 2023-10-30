#include "../Server.hpp"


void Server::invite(int fd, std::stringstream& iss){
	
	std::string name;
	std::string channel;
	std::map<int, Client>::iterator it;
	it = this->list.find(fd);
	iss >> channel;
	iss >> name;

	if (channel.empty() || name.empty())
		throw "Error :Invalid parameteres\r\n";
	if (channel[0] != '#')
		throw "Error :invalid channel name\r\n";;
	if (this->_channels.find(channel) == this->_channels.end()){
		this->sendMessage(fd, "403 " + this->list[fd].getNick() + " " + channel + " :No such channel\r\n");
		return ;
	}
	if (this->_channels[channel].clientExist(this->list.find(fd)->second) == false){
		this->sendMessage(fd, "442 " + this->list[fd].getNick() + " " + channel + " :You're not on that channel\r\n");
		return ;
	}
	if (this->_channels[channel].isOperator(fd) == false){
		this->sendMessage(fd, "482 " + this->list[fd].getNick() + " " + channel + " :You're not channel operator\r\n");
		return ;
	}
	int tmp_fd = this->findClient(name);
	if (tmp_fd == -1){
		this->sendMessage(fd, "401 " + this->list[fd].getNick() + " " + name + " :No such nick\r\n");
		return ;
	}
	if (this->_channels[channel].clientExist(this->list.find(tmp_fd)->second) == true) {
		this->sendMessage(fd, "443 " + this->list[fd].getNick() + " " + name + " " + channel + " :is already on channel\r\n");
		return ;
	}
	if(this->_channels[name].isLimited() && this->_channels[name].isFull()){
		this->sendMessage(fd, "471 " + this->list[fd].getNick() + " " + channel + " :Cannot join channel (+l)\r\n");
		return ;
	}

	this->_channels[channel].addClient(this->list.find(tmp_fd)->second);
	this->_channels[channel].sendMessage(":" + this->list[tmp_fd].getNick() + " JOIN " + channel + "\r\n", -1);
	this->sendMessage(tmp_fd, ":" + this->_hostname + " 332 " + it->second.getNick() + " " + channel + " :" + this->_channels[channel].getTopic() + "\r\n");
	this->sendMessage(tmp_fd, ":" + this->_hostname + " 353 " + it->second.getNick() + " = " + channel + " :" + this->_channels[channel].getUsers() + "\r\n");
	this->sendMessage(tmp_fd, ":" + this->_hostname + " 366 " + it->second.getNick() + " " + channel + " :End of /NAMES list\r\n");
	std::string welcomeMsg = "Welcome to channel " + channel + "!\r\n";
    std::string formattedMsg = ":" + this->list[fd].getNick() + "!~" + this->list[fd].getUser() + "@host PRIVMSG " + channel + " :" + welcomeMsg;
    this->sendMessage(tmp_fd, formattedMsg);
	// this->sendMessage(fd, ); // inform of the success of the invitation
}


