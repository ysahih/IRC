#include "../Server.hpp"

void Server::kick(int fd, std::stringstream& iss){
	
	std::string name;
	std::string channel;
	iss >> channel;
	iss >> name;
	
	channel = toLower(channel);
	name = toLower(name);
	if (channel.empty() || name.empty()){
		this->sendMessage(fd, "Error :invalid parameters\r\n");
		return ;
	}
	if (channel[0] != '#'){
		this->sendMessage(fd, "Error :invalid channel name\r\n");
		return ;
	}
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
		this->sendMessage(fd, "Error :Can't kick channel owner\r\n");
		return ;
	}
	if (tmp_fd == fd){
		this->sendMessage(fd, "Error :You can't kick yourself\r\n");
		return ;
	}

	this->_channels[channel].sendMessage(":" + this->list[fd].getNick() + " KICK " + channel + " " + this->list[tmp_fd].getNick() + " :" + "Kicked by " + this->list[fd].getNick() + "\r\n", -1);
	this->_channels[channel].kickOperator(tmp_fd);
	this->_channels[channel].kickClient(name);
}