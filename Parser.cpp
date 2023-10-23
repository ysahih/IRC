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

void Server::sendMessage(int fd, const char *msg){

    int bytesSent = send(fd, msg, strlen(msg), 0); //!!use of c functions
    if (bytesSent < 0) 
        throw "Failed to send response";
}


void Server::privateMsg(int fd, std::stringstream& iss){

	if (this->list.find(fd)->second.is_authenticate() == false)
		throw "not authenticate\n";
	std::string name;
	iss >> name;
	if (name.empty())
		throw "invalid parameteres\n";
	if (name[0] == '#'){
		//send to channel
		std::map<std::string, Channel>::iterator it = this->_channels.find(name);
		if (it == this->_channels.end())
			throw "channel not found\n";
		if (it->second.clientExist(this->list.find(fd)->second) == false)
			throw "not in channel\n";
		std::string msg;
		getline(iss, msg); // erase first spaces..
		msg += "\n";
		if (msg.empty())
			throw "invalid parameteres\n";
		it->second.sendMessage(msg);
	}//send to channel
	else {
		int tmp_fd = this->findClient(name);
		if (tmp_fd == -1)
			throw "client not found\n";
		std::string msg;
		getline(iss, msg);
		// erase first spaces..
		msg += "\n";
		if (msg.empty())
			throw "invalid parameteres\n";
		this->sendMessage(tmp_fd, msg.c_str());
	}

}

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
		this->_channels[name].sendMessage(it->second.getNick() + " joined the channel\n");
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

	if (name.empty() ) //need to check
		throw "invalid parameteres\n";
	if (this->findClient(name) != -1)
		throw "nick already exist\n";
	it->second.setNick(name);
	it->second.authenticate();
}

void Server::parse(int fd, std::string line){

	std::string str;
	std::stringstream iss(line);
	// !!commands will be switched here :
	std::string cmd[10] = {"NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"};
	iss >> str;
	int index = -1;
    for (int i = 0; i < 10; ++i) {
        if (cmd[i] == str) {
            index = i;
            break;
        }
    }
	switch(index){
		//next stream may be parsed here
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
		// case 4:
		default:
			throw "invalid command\n";
	}
}