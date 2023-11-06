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

	channel = toLower(channel);
	if ((mode[0] != '+' && mode[0] != '-') || mode.length() != 2){
		this->sendMessage(fd, "Error :invalid mode\r\n");
		return ;
	}
	if (channel.empty() || mode.empty()){
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
	int tmp_fd = this->findClient(toLower(word));
	if (mode[0] == '+') {
		switch (mode[1])
		{
			case 'i':
				if (!word.empty()){
					this->sendMessage(fd, "Error :invalid parameteres\r\n");
					return ;
				}
				this->_channels[channel].setPrivate(true);
				this->_channels[channel].sendMessage("MODE " + channel + " +i\r\n", -1);
				break;
			case 'k':
				if (word.empty() && this->_channels[channel].getPassword().empty()){
					this->sendMessage(fd, "Error :invalid parameteres\r\n");
					return ;
				}
				if (!word.empty())
					this->_channels[channel].setPassword(word);
				this->_channels[channel].setLocked(true);
				this->_channels[channel].sendMessage("MODE " + channel + " +k " + word + "\r\n", -1);
				break;
			case 'l':
				if (word.empty()){
					this->sendMessage(fd, "Error :invalid parameteres\r\n");
					return ;
				}
				this->_channels[channel].setSizeLimit(atoi(word.c_str()));
				this->_channels[channel].setLimited(true);
				this->_channels[channel].sendMessage("MODE " + channel + " +l " + word + "\r\n", -1);
				break;
			case 'o':
				if (word.empty()){
					this->sendMessage(fd, "Error :invalid parameteres\r\n");
					return ;
				}
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
				this->_channels[channel].sendMessage("MODE " + channel + " +o " + word + "\r\n", -1);
				break;

			case 't':
				if (!word.empty()){
					this->sendMessage(fd, "Error :invalid parameteres\r\n");
					return ;
				}
				if (this->_channels[channel].getOwner() != fd){
					this->sendMessage(fd, "Error :You're not channel operator\r\n");
					return ;
				}
				this->_channels[channel].topicRestriction(true);
				this->_channels[channel].sendMessage("MODE " + channel + " +t\r\n", -1);
				break;

			default:
				this->sendMessage(fd, "Error :Unknown mode\r\n");
		}
	}
	else if (mode[0] == '-') {
		switch (mode[1])
		{
			case 'i':
				if (!word.empty()){
					this->sendMessage(fd, "Error :invalid parameteres\r\n");
					return ;
				}
				this->_channels[channel].setPrivate(false);
				this->_channels[channel].sendMessage("MODE " + channel + " -i\r\n", -1);
				break;
			case 'k':
				if (!word.empty()){
					this->sendMessage(fd, "Error :invalid parameteres\r\n");
					return ;
				}
				this->_channels[channel].setLocked(false);
				this->_channels[channel].setPassword("");
				this->_channels[channel].sendMessage("MODE " + channel + " -k\r\n", -1);
				break;
			case 'l':
				if (!word.empty()){
					this->sendMessage(fd, "Error :invalid parameteres\r\n");
					return ;
				}
				this->_channels[channel].setSizeLimit(INT_MAX);
				this->_channels[channel].setLimited(false);
				this->_channels[channel].sendMessage("MODE " + channel + " -l\r\n", -1);
				break;
			case 'o':
				if(word.empty()){
					this->sendMessage(fd, "Error :invalid parameteres\r\n");
					return ;
				}
				if (tmp_fd == -1){
					this->sendMessage(fd, "401 " + this->list[fd].getNick() + " " + word + " :No such nick\r\n");
					return ;
				}
				if (this->_channels[channel].clientExist(this->list.find(tmp_fd)->second) == false){
					this->sendMessage(fd, "441 " + this->list[fd].getNick() + " " + word + " " + channel + " :They aren't on that channel\r\n");
					return ;
				}
				if (this->_channels[channel].isOperator(tmp_fd) == false){
					this->sendMessage(fd, "Error :They are not operator\r\n");
					return ;
				}
				if (this->_channels[channel].getOwner() == tmp_fd){
					this->sendMessage(fd, "Error :You can't remove owner\r\n");
					return ;
				}
				if (tmp_fd == fd){
					this->sendMessage(fd, "Error :You can't remove yourself\r\n");
					return ;
				}
				this->_channels[channel].kickOperator(tmp_fd);
				this->_channels[channel].sendMessage("MODE " + channel + " -o " + word + "\r\n", -1);
				break;

			case 't':
				if (!word.empty()){
					this->sendMessage(fd, "Error :invalid parameteres\r\n");
					return ;
				}
				if (this->_channels[channel].getOwner() != fd){
					this->sendMessage(fd, "Error :You're not channel operator\r\n");
					return ;
				}
				this->_channels[channel].topicRestriction(false);
				this->_channels[channel].sendMessage("MODE " + channel + " -t\r\n", -1);
				break;

			default:
				this->sendMessage(fd, "Error :Unknown mode\r\n");
		}
	}
	
}
