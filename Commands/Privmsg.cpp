#include "../Server.hpp"

void Server::privateMsg(int fd, std::stringstream& iss){

	std::string name;
	std::string msg;
	iss >> name;
	getline(iss, msg);
	
	if (this->list.find(fd)->second.isAuthenticate() == false){
		this->sendMessage(fd, "Error :You are not authenticated\r\n");
		return ;
	}
	if (name.empty() || msg.empty()){
		this->sendMessage(fd, "461 " + this->list[fd].getNick() + " PRIVMSG :Not enough parameters\r\n");
		return ;
	}
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
		it->second.sendMessage(":" + this->list[fd].getNick() + " PRIVMSG " + name + " :" + msg + "\r\n", fd);
	}

	else { //send to client
		int tmp_fd = this->findClient(name);
		if (tmp_fd == -1){
			this->sendMessage(fd, "401 " + this->list[fd].getNick() + " " + name + " :No such nick\r\n");
			return ;
		}
		this->sendMessage(tmp_fd, ":" + this->list[fd].getNick() + " PRIVMSG " + name + " :" + msg + "\r\n");
	}

}