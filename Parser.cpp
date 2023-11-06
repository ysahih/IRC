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

    int bytesSent = write(fd, msg.c_str(), msg.length());
    if (bytesSent < 0)
        throw std::runtime_error("Failed to send response");
}
std::string toLower(std::string str){
	std::string tmp = "";
	for (size_t i = 0; i < str.size(); i++){
		tmp += std::tolower(str[i]);
	}
	return tmp;
}

std::map<std::string, std::string> collectChannels(std::string names, std::string passwords) {
	std::map<std::string, std::string> channels;
	std::string name;
	std::string pass;
	std::stringstream iss(names);
	std::stringstream pss(passwords);

	while (std::getline(iss, name, ',')) {

		std::getline(pss, pass, ',');
		channels[toLower(name)] = pass;
		if (iss.eof() && !pss.eof())
			return std::map<std::string, std::string>();
		if (pss.eof())
			pass = "";
	}
	return channels;
}

void Server::part(int fd, std::stringstream& iss){
	std::string names;
	std::string msg;
	std::map<int, Client>::iterator it = this->list.find(fd);
	iss >> names;
	iss >> msg;
	if (names.empty()){
		this->sendMessage(fd, "Error :invalid parameters\r\n");
		return ;
	}
	std::map<std::string, std::string> channels = collectChannels(names, msg);
	std::map<std::string, std::string>::iterator it2 = channels.begin();
	if (it2 == channels.end()){
		this->sendMessage(fd, "Error :invalid parameters\r\n");
		return ;
	}
	for (; it2 != channels.end(); it2++) {
		if (it2->first[0] != '#') {
			this->sendMessage(fd, "Error :invalid channel name\r\n");
			return ;
		}
		if (this->_channels.find(it2->first) == this->_channels.end()){
			this->sendMessage(fd, "442 " + it->second.getNick() + " " + it2->first + " :You're not on that channel\r\n");
			return ;
		}
		if (!this->_channels[it2->first].clientExist(it->second)){
			this->sendMessage(fd, "442 " + it->second.getNick() + " " + it2->first + " :You're not on that channel\r\n");
			return ;
		}
		this->_channels[it2->first].sendMessage(":" + it->second.getNick() + " PART " + it2->first + " :" + msg + "\r\n", -1);
		this->_channels[it2->first].kickClient(it->second.getNick());
		this->_channels[it2->first].kickOperator(fd);
		this->_channels[it2->first].sendMessage(":" + this->_hostname + " 353 " + it->second.getNick() + " = " + it2->first + " :" + this->_channels[it2->first].getUsers() + "\r\n", -1);
		this->_channels[it2->first].sendMessage(":" + this->_hostname + " 366 " + it->second.getNick() + " " + it2->first + " :End of /NAMES list\r\n", -1);
	}
}

void Server::quit(int fd){
	std::map<int, Client>::iterator it = this->list.find(fd);
	std::map<std::string, Channel>::iterator it2 = this->_channels.begin();
	for (; it2 != this->_channels.end(); it2++){
		if (it2->second.clientExist(it->second))
			it2->second.kickClient(it->second.getNick());
		else if (it2->second.isOperator(fd))
			it2->second.kickOperator(it->first);
		it2->second.sendMessage(":" + this->list[fd].getNick() + " KICK " + it2->first + " " + this->list[fd].getNick() + " :" + "Was kicked by !\r\n", -1);
	}
	this->list.erase(it);
	close(fd);
}

void Server::parse(int fd, std::string line){

	std::string str;
	std::stringstream iss(line);
	std::string cmd[12] = {"NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "PONG", "QUIT", "PART", "BOT"};
	iss >> str;
	int index = -1;
    for (int i = 0; i < 12; ++i) {
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
		case 8:
			break;
		case 9: 
			this->quit(fd);
			break;
		case 10:
			this->part(fd, iss);
			break;
		case 11:
			this->bot(fd, iss);
			break;
		default:
			this->sendMessage (fd, "421 " + str + " :Unknown command\r\n");
	}
}
