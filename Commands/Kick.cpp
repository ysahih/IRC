#include "../Server.hpp"

void Server::kick(int fd, std::stringstream& iss){
	
	std::string name;
	std::string channel;
	iss >> channel;
	iss >> name;
	

	if (channel.empty() || name.empty())
		throw  "Error :Invalid parameteres\r\n";
	if (channel[0] != '#')
		throw "Error :invalid channel name\r\n";
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
	if (this->_channels[channel].clientExist(this->list.find(tmp_fd)->second) == false){
		this->sendMessage(fd, "441 " + name + " " + channel + " :They aren't on that channel\r\n");
		return ;
	}
	if (this->_channels[channel].isOperator(tmp_fd) == true && this->_channels[channel].getOwner() != fd){
		this->sendMessage(fd, "Error :You're not channel operator\r\n");
		return ;
	}
	
	this->sendMessage(tmp_fd, ":" + this->list[fd].getNick() + " KICK " + channel + " " + this->list[tmp_fd].getNick() + " :" + "Kicked by " + this->list[fd].getNick() + "\r\n");
	this->_channels[channel].kickOperator(tmp_fd);
	this->_channels[channel].kickClient(this->list.find(tmp_fd)->second.getNick());
	this->sendMessage(fd, ":" + this->list[fd].getNick() + " KICK " + channel + " " + this->list[tmp_fd].getNick() + " :" + "Kicked by " + this->list[fd].getNick() + "\r\n");
}