#include "Server.hpp"


int Server::findClient(std::string name){
    std::map<int, Client>::iterator it = this->list.begin();
    for (; it != this->list.end(); it++){
        if (it->second.getNick() == name)
            return it->first;
    }
    return -1;
}

void Server::sendMessage(int fd, std::string msg){

    int bytesSent = send(fd, msg.c_str(), msg.length(), 0);
    if (bytesSent < 0) 
        throw "Failed to send response"; //! this error should kill the server
}


void Server::privateMsg(int fd, std::stringstream& iss){

	std::string name;
	iss >> name;
	if (this->list.find(fd)->second.isAuthenticate() == false){
		this->sendMessage(fd, "Error :You are not authenticated\r\n");
		return ;
	}
	if (name.empty())
		throw "Error :invalid parameteres\r\n";
	if (name[0] == '#'){ //send to channel
		std::map<std::string, Channel>::iterator it = this->_channels.find(name);
		if (it == this->_channels.end()){
			this->sendMessage(fd, "403 " + this->list[fd].getNick() + " " + name + " :No such channel\r\n");
			return ;
		}
		if (it->second.clientExist(this->list.find(fd)->second) == false){
			this->sendMessage(fd, "442 " + this->list[fd].getNick() + " " + name + " :You're not on that channel\r\n");
			return ;
		}
		std::string msg;
		getline(iss, msg);
		if (msg.empty())
			throw "Error :invalid parameteres\r\n";
		msg += "\n";
		it->second.sendMessage(this->list[fd].getNick() + ":" + msg);
	}

	else { //send to client
		int tmp_fd = this->findClient(name);
		if (tmp_fd == -1){
			this->sendMessage(fd, "401 " + this->list[fd].getNick() + " " + name + " :No such nick\r\n");
			return ;
		}
		std::string msg;
		getline(iss, msg);
		msg += "\r\n";
		if (msg.empty())
			throw "Error :invalid parameteres\r\n";
		this->sendMessage(fd, this->list[fd].getNick() + ":" + msg.c_str());
		this->sendMessage(tmp_fd, this->list[fd].getNick() + ":" + msg.c_str());

	}

}

std::map<std::string, std::string> collectChannels(std::string names, std::string passwords) {
	std::map<std::string, std::string> channels;
	std::string name;
	std::string pass;
	std::stringstream iss(names);
	std::stringstream pss(passwords);

	while (std::getline(iss, name, ',')) {

		std::getline(pss, pass, ',');
		channels[name] = pass;
		if (iss.eof() && !pss.eof())
			throw "Error :invalid parameters\r\n";
		if (pss.eof())
			pass = "";
	}
	return channels;
}

void Server::joinChannel(int fd, std::stringstream& iss) {
	
	std::string names;
	std::string passwords;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> names;
	iss >> passwords;


	if(it->second.isAuthenticate() == false)
		throw "Error :You are not authenticated\r\n";


	if (names.empty())
		throw "Error :invalid parameters\r\n";
	std::map<std::string, std::string> channels = collectChannels(names, passwords);
	std::map<std::string, std::string>::iterator it2 = channels.begin();


	for(; it2 != channels.end(); it2++) {
		if (it2->first[0] != '#')
			throw "Error :invalid channel name\r\n";
		if (this->_channels.find(it2->first) == this->_channels.end()){

			Channel newChannel(it2->first);
			newChannel.addClient(it->second);
			newChannel.setOperator(fd);
			newChannel.setPassword(it2->second);
			this->_channels[it2->first] = newChannel;
			this->sendMessage(fd, ":" + this->_hostname + " 331 " + it->second.getNick() + " " + it2->first + " :No topic is set\r\n");
			this->sendMessage(fd, ":" + this->_hostname + " 353 " + it->second.getNick() + " = " + it2->first + " :" + this->_channels[it2->first].getUsers() + "\r\n");
			this->sendMessage(fd, ":" + this->_hostname + " 366 " + it->second.getNick() + " " + it2->first + " :End of /NAMES list\r\n");
			// std::string welcomeMsg = "Welcome to channel " + it2->first + "!\r\n";
    		// std::string formattedMsg = ":" + this->list[fd].getNick() + "!~" + this->list[fd].getUser() + "@host PRIVMSG " + it2->first + " :" + welcomeMsg;
    		// this->_channels[it2->first].sendMessage(formattedMsg);

		}
		else {
			if (this->_channels[it2->first].clientExist(it->second)){
				this->sendMessage(fd, "442 " + it->second.getNick() + " " + it2->first + " :You're already on that channel\r\n");
				return ;
			}
			if (this->_channels[it2->first].isPrivate()){
				this->sendMessage(fd, "473 " + it->second.getNick() + " " + it2->first + " :Cannot join channel (+i)\r\n");
				return ;
			}
			if(this->_channels[it2->first].isLimited() && this->_channels[it2->first].isFull()){
				this->sendMessage(fd, "471 " + it->second.getNick() + " " + it2->first + " :Cannot join channel (+l)\r\n");
				return ;
			}
			if (this->_channels[it2->first].isLocked() && this->_channels[it2->first].getPassword() != it2->second){
				this->sendMessage(fd, "464 " + it->second.getNick() + " " + it2->first + " :Password incorrect\r\n");
				return ;
			}

			this->_channels[it2->first].sendMessage(it->second.getNick() + " has joined channel: " + it2->first + "\r\n");
			this->_channels[it2->first].addClient(it->second);
			this->sendMessage(fd, ":" + this->_hostname + " 332 " + it->second.getNick() + " " + it2->first + " :" + this->_channels[it2->first].getTopic() + "\r\n");
			this->sendMessage(fd, ":" + this->_hostname + " 353 " + it->second.getNick() + " = " + it2->first + " :" + this->_channels[it2->first].getUsers() + "\r\n");
			this->sendMessage(fd, ":" + this->_hostname + " 366 " + it->second.getNick() + " " + it2->first + " :End of /NAMES list\r\n");
			// std::string welcomeMsg = "Welcome to channel " + it2->first + "!\r\n";
    		// std::string formattedMsg = ":" + this->list[fd].getNick() + "!~" + this->list[fd].getUser() + "@host PRIVMSG " + it2->first + " :" + welcomeMsg;
    		// this->_channels[it2->first].sendMessage(formattedMsg);

		}
	}
}

void Server::setUser(int fd, std::stringstream& iss) {

	std::string name;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;


	if (name.empty() || name == ":"){
		this->sendMessage(fd, "ERROR :No nickname given\r\n");
		return ;
	}
	it->second.setUser(name);
	it->second.authenticate();
	if (it->second.isWelcomed() == false && it->second.isAuthenticate() == true){
		this->sendMessage(fd, ":" + this->_hostname + " 001  " + name + " :Welcome to the IRC Network, " + name + "!\r\n");
		it->second.setWelcomed(true);
	}
}

void Server::setNick(int fd, std::stringstream& iss) {
	
	std::string name;
	std::string tmp;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;

	if (iss >> tmp){
		this->sendMessage(fd, "Error :Invalid parameteres\r\n");
		return ;
	}
	if (name.empty() || name == ":"){
		this->sendMessage(fd, "ERROR :No nickname given\r\n");
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

void Server::kick(int fd, std::stringstream& iss){
	
	std::string name;
	std::string channel;
	std::map<int, Client>::iterator it;
	it = this->list.find(fd);
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
		this->sendMessage(fd, "441 " + this->list[fd].getNick() + " " + name + " " + channel + " :They aren't on that channel\r\n");
		return ;
	}
	if (this->_channels[channel].isOperator(tmp_fd) == true && this->_channels[channel].getOwner() != fd){
		this->sendMessage(fd, "Error :You're not channel operator\r\n");
		return ;
	}
	this->sendMessage(tmp_fd, "KICK " + channel + " " + name + " :You have been kicked by " + this->list[fd].getNick() + "\r\n");
	this->_channels[channel].kickOperator(tmp_fd);
	this->_channels[channel].kickClient(this->list.find(tmp_fd)->second.getNick());
	this->_channels[channel].sendMessage(name + " was kicked from channel: " + channel + "\r\n");
}

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
	this->_channels[channel].sendMessage(name + " was invited to channel: " + channel + "\r\n");
	this->sendMessage(tmp_fd, ":" + this->_hostname + " INVITE " + name + " " + channel + "\r\n"); // inform of the invitation
	this->sendMessage(fd, ":" + this->_hostname + " 341 " + this->list[fd].getNick() + " " + name + " " + channel + "\r\n"); // inform of the success of the invitation
	
}

void Server::topic(int fd, std::stringstream& iss){
	
	std::string topic;
	std::string channel;
	std::map<int, Client>::iterator it;
	it = this->list.find(fd);
	iss >> channel;
	iss >> topic;

	if (channel.empty())
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
	this->_channels[channel].sendMessage(this->list[fd].getNick() + " has changed topic to: " + topic + "\r\n");
	this->sendMessage(fd, ":" + this->_hostname + " 332 " + this->list[fd].getNick() + " " + channel + " :" + this->_channels[channel].getTopic() + "\r\n"); // inform of the success of setting the topic
}

void Server::mode(int fd, std::stringstream& iss){
	std::string mode;
	std::string channel;
	std::string word;
	std::map<int, Client>::iterator it;
	it = this->list.find(fd);
	iss >> mode;
	iss >> channel;
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

void Server::parse(int fd, std::string line){

	std::string str;
	std::stringstream iss(line);
	std::string cmd[10] = {"NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"};
	iss >> str;
	int index = -1;
    for (int i = 0; i < 10; ++i) {
        if (cmd[i] == str) {
            index = i;
            break;
        }
    }
	switch(index) {
		case 0:
			this->setNick(fd, iss);
			break;
		case 1:
			this->setUser(fd, iss);
			break;
		case 2:
			this->joinChannel(fd, iss);
			break;
		case 3:
			this->privateMsg(fd, iss);
			break;
		case 4:
			this->kick(fd, iss);
			break;
		case 5:
			this->invite(fd, iss);
			break;
		case 6:
			this->topic(fd, iss);
			break;
		case 7:
			this->mode(fd, iss);
			break;
		default:
			this->sendMessage (fd, "421 " + str + " :Unknown command\r\n");
			// throw "Error :invalid cmd\r\n";
	}
}


