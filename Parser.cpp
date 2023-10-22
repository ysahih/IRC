#include "Server.hpp"


void Server::joinChannel(int fd, std::stringstream& iss){
	
	std::string name;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;

	if (name.empty())
		throw "invalid parameteres\n";
	if (name[0] != '#')
		throw "invalid channel name\n";
	if(!it->second.is_authenticate())
		throw "not authenticate\n";
	
	if (this->_channels.find(name) == this->_channels.end()){

		Channel newChannel(name);
		newChannel.addClient(it->second);
		newChannel.setOperator(fd);
		this->_channels[name] = newChannel;
		this->sendMessage(fd, "joined channel as operator\n");
	}
	else {
		if (this->_channels[name].clientExist(it->second))
			throw "already in channel\n";
		this->_channels[name].addClient(it->second);
		this->sendMessage(fd, "joined channel\n");
	}
}


void Server::setUser(int fd, std::stringstream& iss) {

	std::string name;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;

	if (name.empty() ) // need to check if the string is empty
		throw "invalid parameteres\n";
	it->second.setUser(name);
	it->second.authenticate();
	this->sendMessage(fd, "user set\n");

}

void Server::setNick(int fd, std::stringstream& iss) {
	
	std::string name;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> name;

	if (name.empty() ) // need to check 
		throw "invalid parameteres\n";
	it->second.setNick(name);
	it->second.authenticate();
}

void Server::parse(int fd, std::string line){

	std::string str;
	std::stringstream iss(line);
	iss >> str;
	// if (this->list.find(fd)->second.is_authenticate() == false)
	// 	throw "not authenticate\n";
	if (str == "PRIVMSG"){
		std::string name;
		iss >> name;
		if (name.empty())
			throw "invalid parameteres\n";
		if (name[0] == '#'){}//send to channel
		else {
			int tmp_fd = this->findClient(name);
			std::string msg;
			getline(iss, msg); // erase first spaces..
			msg += "\n";
			if (msg.empty())
				throw "invalid parameteres\n";
			this->sendMessage(tmp_fd, msg.c_str());
		}
	}
	else if (str == "NICK")
		this->setNick(fd, iss);
	else if (str == "USER")
		this->setUser(fd, iss);
	else if (str == "JOIN")
		this->joinChannel(fd, iss);
	else
		throw "invalid command\n";

}