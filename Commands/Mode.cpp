#include "../Server.hpp"

void Server::mode(int fd, std::stringstream& iss){
	std::string mode;
	std::string channel;
	std::string word;
	std::map<int, Client>::iterator it;
	it = this->list.find(fd);
	iss >> channel;
	iss >> mode;
	iss >> word;
	if (mode[0] != '+' && mode[0] != '-')
		throw "Error :Unknown mode type\r\n";
	if (channel.empty() || mode.empty())
		throw "Error :invalid parameteres\r\n";
	if (channel[0] != '#')
		throw "Error :invalid channel name\n";
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
	int tmp_fd = this->findClient(word);
	if (mode[0] == '+') {
		switch (mode[1])
		{
			case 'i':
				this->_channels[channel].setPrivate(true);
				this->sendMessage(fd, "MODE " + channel + " +i\r\n");
				break;
			case 'k':
				if (word.empty() && this->_channels[channel].getPassword().empty())
					throw "Error :No password was given\r\n";
				if (!word.empty())
					this->_channels[channel].setPassword(word);
				this->_channels[channel].setLocked(true);
				this->sendMessage(fd, "MODE " + channel + " +k " + word + "\r\n");
				break;
			case 'l':
				if (word.empty())
					throw "Error :invalid parameteres\r\n";
				this->_channels[channel].setSizeLimit(atoi(word.c_str()));
				this->_channels[channel].setLimited(true);
				this->sendMessage(fd, "MODE " + channel + " +l " + word + "\r\n");
				break;
			case 'o':
				if (word.empty())
					throw "Error :invalid parameteres\r\n";
				if (tmp_fd == -1){
					this->sendMessage(fd, "401 " + this->list[fd].getNick() + " " + word + " :No such nick\r\n");
					return ;
				}
				if (this->_channels[channel].clientExist(this->list.find(tmp_fd)->second) == false){
					this->sendMessage(fd, "441 " + this->list[fd].getNick() + " " + word + " " + channel + " :They aren't on that channel\r\n");
					return ;
				}
				if (this->_channels[channel].isOperator(tmp_fd) == true){
					this->sendMessage(fd, "Error :They are already operator\r\n");
					return ;
				}
				this->_channels[channel].setOperator(tmp_fd);
				this->sendMessage(fd, "MODE " + channel + " +o " + word + "\r\n");
				break;

			case 't':
				if (this->_channels[channel].getOwner() != fd){
					this->sendMessage(fd, "Error :You're not channel operator\r\n");
					return ;
				}
				this->_channels[channel].topicRestriction(true);
				this->sendMessage(fd, "MODE " + channel + " +t\r\n");
				break;

			default:
				throw "Error :Unknown mode\n";
		}
	}
	else if (mode[0] == '-') {
		if (!word.empty())
			throw "Error :invalid parameteres\r\n";
		switch (mode[1])
		{
			case 'i':
				this->_channels[channel].setPrivate(false);
				this->sendMessage(fd, "MODE " + channel + " -i\r\n");
				break;
			case 'k':
				this->_channels[channel].setLocked(false);
				this->_channels[channel].setPassword("");
				this->sendMessage(fd, "MODE " + channel + " -k\r\n");
				break;
			case 'l':
				this->_channels[channel].setSizeLimit(INT_MAX);
				this->_channels[channel].setLimited(false);
				this->sendMessage(fd, "MODE " + channel + " -l\r\n");
				break;
			case 'o':
				if (tmp_fd == -1){
					this->sendMessage(fd, "401 " + this->list[fd].getNick() + " " + word + " :No such nick\r\n");
					return ;
				}
				if (this->_channels[channel].clientExist(this->list.find(tmp_fd)->second) == false){
					this->sendMessage(fd, "441 " + this->list[fd].getNick() + " " + word + " " + channel + " :They aren't on that channel\r\n");
					return ;
				}
				if (this->_channels[channel].getOwner() == fd){
					this->sendMessage(fd, "Error :You're not channel operator\r\n");
					return ;
				}
				this->_channels[channel].kickOperator(tmp_fd);
				this->sendMessage(fd, "MODE " + channel + " -o " + word + "\r\n");
				break;

			case 't':
				if (this->_channels[channel].getOwner() != fd){
					this->sendMessage(fd, "Error :You're not channel operator\r\n");
					return ;
				}
				this->_channels[channel].topicRestriction(false);
				this->sendMessage(fd, "MODE " + channel + " -t\r\n");
				break;

			default:
				throw "Error :Unknown mode\n";
		}
	}
	
}
