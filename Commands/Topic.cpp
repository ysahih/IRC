#include "../Server.hpp"


void Server::topic(int fd, std::stringstream& iss){
	
	std::string topic;
	std::string channel;
	std::map<int, Client>::iterator it;
	it = this->list.find(fd);
	iss >> channel;
	iss >> topic;

	if (channel.empty()){
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
	}
	if (this->_channels[channel].isTopicRestricted() == true && this->_channels[channel].getOwner() != fd){
		this->sendMessage(fd, "Error :You're not channel operator\r\n");
		return ;
	}
	if (topic.empty())
	{
		if (this->_channels[channel].getTopic().empty())
			this->sendMessage(fd, ":" + this->_hostname + " 331 " + this->list[fd].getNick() + " " + channel + " :No topic is set\r\n");
		else
			this->sendMessage(fd, ":" + this->_hostname + " 332 " + this->list[fd].getNick() + " " + channel + " :" + this->_channels[channel].getTopic() + "\r\n");
		return;
	}
	this->_channels[channel].setTopic(topic);
	this->_channels[channel].sendMessage(this->list[fd].getNick() + " has changed topic to: " + topic + "\r\n", fd);
	this->sendMessage(fd, ":" + this->_hostname + " 332 " + this->list[fd].getNick() + " " + channel + " :" + this->_channels[channel].getTopic() + "\r\n"); // inform of the success of setting the topic
}
