#include "../Server.hpp"


std::vector<std::string> nameSplit(std::string name){
	std::vector<std::string> names;
	std::string tmp;
	std::stringstream ss(name);
	while (getline(ss, tmp, ',')){
		names.push_back(tmp);
	}
	for(size_t i = 0; i < names.size(); i++)
		std::cout << names[i] << std::endl;
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
		this->sendMessage(fd, "461 " + this->list[fd].getNick() + " PRIVMSG :Not enough parameters\r\n");//!
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
				this->sendMessage(fd, "403 " + this->list[fd].getNick() + " " + name + " :No such channel\r\n");
				return ;
			}
			if (it->second.clientExist(this->list.find(fd)->second) == false){
				this->sendMessage(fd, "442 " + this->list[fd].getNick() + " " + name + " :You're not on that channel\r\n");
				return ;
			}
			it->second.sendMessage(":" + this->list[fd].getNick() + "@" + this->list[fd].getIP() + " PRIVMSG " + name + " :" + msg + "\r\n", fd);
		}

		else { //send to client
			int tmp_fd = this->findClient(name);
			if (tmp_fd == -1){
				this->sendMessage(fd, "401 " + this->list[fd].getNick() + " " + name + " :No such nick\r\n");
				return ;
			}
			// :yahya!ss@(hostname) PRIVMSG saad :slm
			//message = ":" + __users[fd].getNickname() + " PRIVMSG " + targetsVec[i] + " " + msg + "\n";
			this->sendMessage(tmp_fd, ":" + this->list[fd].getNick() + "!" + this->list[fd].getUser() + "@" + _hostname +  " PRIVMSG " + this->list[tmp_fd].getNick() + " :" + msg + "\r\n");
			// std::cout << ":" + this->list[fd].getNick() + "!~" + this->list[fd].getUser() + "@" + _hostname +  " PRIVMSG " + this->list[tmp_fd].getNick() + " :" + msg + "\r\n";
			// this->sendMessage(tmp_fd, ":" + this->list[fd].getNick() + " PRIVMSG " + this->list[tmp_fd].getNick() + " "  + msg + "\n");
		}
	}

}