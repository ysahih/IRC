#include "Server.hpp"


int Server::findClient(std::string name){
    // std::cout << '-' + name + '-' << std::endl;
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
	if (this->list.find(fd)->second.is_authenticate() == false)
		throw "not authenticate\n";
	if (name.empty())
		throw "invalid parameteres\n";
	if (name[0] == '#'){ //send to channel
		std::map<std::string, Channel>::iterator it = this->_channels.find(name);
		if (it == this->_channels.end())
			throw "channel not found\n";
		if (it->second.clientExist(this->list.find(fd)->second) == false)
			throw "not in channel\n";
		
		//msg sent here and to users must be parsed
		std::string msg;
		getline(iss, msg);
		if (msg.empty())
			throw "invalid parameteres\n";
		msg += "\n";
		it->second.sendMessage(this->list[fd].getNick() + ":" + msg);
	}
	else { //send to client
		int tmp_fd = this->findClient(name);
		if (tmp_fd == -1)
			throw "client not found\n";
		std::string msg;
		getline(iss, msg);
		msg += "\n";
		if (msg.empty())
			throw "invalid parameteres\n";
		this->sendMessage(tmp_fd, this->list[fd].getNick() + ":" + msg.c_str());
	}

}

void Server::joinChannel(int fd, std::stringstream& iss) {
	
	std::string name;
	std::string password;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;
	iss >> password;

	if(!it->second.is_authenticate())
		throw "not authenticate\n";
	if (name.empty())
		throw "invalid parameteres\n";
	while (true){
		if (name[0] != '#')
			throw "invalid channel name\n";
		if (this->_channels.find(name) == this->_channels.end()){

			Channel newChannel(name);
			newChannel.addClient(it->second);
			newChannel.setOperator(fd);
			newChannel.setPassword(password);
			this->_channels[name] = newChannel;
			this->_channels[name].sendMessage(it->second.getNick() + " joined the channel: " + name + " as operator\n");
		}
		else {
			if (this->_channels[name].clientExist(it->second))
				throw "already in channel\n";
			if (this->_channels[name].isPrivate())
				throw "channel is invite only\n";
			if(this->_channels[name].isLimited() && this->_channels[name].isFull())
				throw "channel is full\n";
			if (!this->_channels[name].isLocked() && this->_channels[name].getPassword() != password)
				throw "invalid password\n";
			this->_channels[name].addClient(it->second);
			this->_channels[name].sendMessage(it->second.getNick() + " joined the channel: " + name + "\n");
		}
		iss >> name;
		if (iss.eof())
			break;
	}
}

void Server::setUser(int fd, std::stringstream& iss) {

	std::string name;
	std::string tmp;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;

	if (iss >> tmp)
		throw "invalid parameteres\n";
	if (name.empty())
		throw "invalid parameteres\n";
	it->second.setUser(name);
	it->second.authenticate();
	this->sendMessage(fd, "user set\n");
}

void Server::setNick(int fd, std::stringstream& iss) {
	
	std::string name;
	std::string tmp;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;

	if (iss >> tmp)
		throw "invalid parameteres\n";
	if (name.empty())
		throw "invalid parameteres\n";
	if (this->findClient(name) != -1)
		throw "nick already exist\n";
	it->second.setNick(name);
	it->second.authenticate();
}

void Server::kick(int fd, std::stringstream& iss){
	
	std::string name;
	std::string channel;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;
	iss >> channel;

	if (channel.empty() || name.empty())
		throw "invalid parameteres\n";
	if (channel[0] != '#')
		throw "invalid channel name\n";
	if (this->_channels.find(channel) == this->_channels.end())
		throw "channel not found\n";
	if (this->_channels[channel].clientExist(this->list.find(fd)->second) == false)
		throw "not in channel\n";
	if (this->_channels[channel].isOperator(fd) == false)
		throw "not operator\n";
	int tmp_fd = this->findClient(name);
	if (tmp_fd == -1)
		throw "client not found\n";
	if (this->_channels[channel].clientExist(this->list.find(tmp_fd)->second) == false)
		throw "client not in channel\n";
	if (this->_channels[channel].isOperator(tmp_fd) == true)
		throw "cannot kick operator\n";
	this->_channels[channel].sendMessage(name + " was kicked from channel: " + channel + "\n");
	this->_channels[channel].kickClient(this->list.find(tmp_fd)->second.getNick());
}

void Server::invite(int fd, std::stringstream& iss){
	
	std::string name;
	std::string channel;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;
	iss >> channel;

	if (channel.empty() || name.empty())
		throw "invalid parameteres\n";
	if (channel[0] != '#')
		throw "invalid channel name\n";
	if (this->_channels.find(channel) == this->_channels.end())
		throw "channel not found\n";
	if (this->_channels[channel].clientExist(this->list.find(fd)->second) == false)
		throw "not in channel\n";
	if (this->_channels[channel].isOperator(fd) == false)
		throw "not operator\n";
	int tmp_fd = this->findClient(name);
	if (tmp_fd == -1)
		throw "client not found\n";
	if (this->_channels[channel].clientExist(this->list.find(tmp_fd)->second) == true)
		throw "client already in channel\n";
	if(this->_channels[name].isLimited() && this->_channels[name].isFull())
				throw "channel is full\n";
	this->_channels[channel].addClient(this->list.find(tmp_fd)->second);
	this->_channels[channel].sendMessage(name + " was invited to channel: " + channel + "\n");
}

void Server::topic(int fd, std::stringstream& iss){
	
	std::string topic;
	std::string channel;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> topic;
	iss >> channel;

	if (channel.empty() || topic.empty())
		throw "invalid parameteres\n";
	if (channel[0] != '#')
		throw "invalid channel name\n";
	if (this->_channels.find(channel) == this->_channels.end())
		throw "channel not found\n";
	if (this->_channels[channel].clientExist(this->list.find(fd)->second) == false)
		throw "not in channel\n";
	if (this->_channels[channel].isOperator(fd) == false)
		throw "not operator\n";
	this->_channels[channel].setTopic(topic);
	this->_channels[channel].sendMessage(channel + " :topic changed to: " + topic + "\n");
	
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
		default:
			throw "invalid command\n";
	}
}