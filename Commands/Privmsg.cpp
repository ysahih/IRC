#include "../Server.hpp"


std::vector<std::string> nameSplit(std::string name){
	std::vector<std::string> names;
	std::string tmp;
	std::stringstream ss(name);
	while (getline(ss, tmp, ',')){
		names.push_back(toLower(tmp));
	}

	return names;
}


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
		this->sendMessage(fd, ":" + this->list[fd].getNick() + "461 PRIVMSG :Not enough parameters\r\n");
		return ;
	}
	if (msg[0] == ':')
		msg = msg.substr(2);
	else
		msg = msg.substr(1);
	std::vector<std::string> names = nameSplit(name);
	for(size_t i=0; i<names.size(); i++){
		name = names[i];
		if (name[0] == '#'){ //send to channel
			std::map<std::string, Channel>::iterator it = this->_channels.find(name);
			if (it == this->_channels.end()){
				this->sendMessage(fd, ":" + this->list[fd].getNick() + " 403 PRIVMSG :No such channel\r\n");
				return ;
			}
			if (it->second.clientExist(this->list.find(fd)->second) == false){
				this->sendMessage(fd, ":" + this->list[fd].getNick() + " 442 PRIVMSG :You're not on that channel\r\n");
				return ;
			}
			it->second.sendMessage(":" + this->list[fd].getNick() + " PRIVMSG " + name + " :" + msg + "\r\n", fd);
		}

		else { //send to client
			int tmp_fd = this->findClient(name);
			if (tmp_fd == -1){
				this->sendMessage(fd, ":" + this->list[fd].getNick() + " 401 PRIVMSG :No such nick\r\n");
				return ;
			}
			if (tmp_fd == fd){
				this->sendMessage(fd, ":" + this->list[fd].getNick() + " 400 PRIVMSG :Cannot send to yourself\r\n");
				return ;
			}
			// this->sendMessage(tmp_fd, ":" + this->list[fd].getNick() + "!" + this->list[fd].getUser() + "@" + _hostname +  " PRIVMSG " + this->list[tmp_fd].getNick() + " :" + msg + "\r\n");
			std::string mssg = ":" + this->list[fd].getNick() + " PRIVMSG " + name + " :"  + msg + "\r\n";
			this->sendMessage(tmp_fd, mssg);
		}
	}

}