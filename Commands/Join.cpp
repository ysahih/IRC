#include "../Server.hpp"


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
			//  ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
			this->sendMessage(fd, ":" + this->list[fd].getNick() + "!~" + this->list[fd].getUser() + "@" + this->list[fd].getIP() + " JOIN :" + it2->first + "\r\n");
			this->_channels[it2->first].sendMessage(":" + this->_hostname + " 332 " + it->second.getNick() + " " + it2->first + " :" + this->_channels[it2->first].getTopic() + "\r\n", -1);
			this->sendMessage(fd, ":" + this->_hostname + " 353 " + it->second.getNick() + " = " + it2->first + " :" + this->_channels[it2->first].getUsers() + "\r\n");
			this->sendMessage(fd, ":" + this->_hostname + " 366 " + it->second.getNick() + " " + it2->first + " :End of /NAMES list\r\n");

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
				this->sendMessage(fd, ":" + it->second.getNick() + " 471 JOIN " + it2->first + " :Cannot join channel (+l)\r\n");
				return ;
			}
			if (this->_channels[it2->first].isLocked() && this->_channels[it2->first].getPassword() != it2->second){
				this->sendMessage(fd, "464 " + it->second.getNick() + " " + it2->first + " :Password incorrect\r\n");
				return ;
			}

			this->_channels[it2->first].addClient(it->second);
			this->sendMessage(fd, ":" + this->list[fd].getNick() + "!~" + this->list[fd].getUser() + "@" + this->list[fd].getIP() + " JOIN :" + it2->first + "\r\n");
			this->_channels[it2->first].sendMessage(":" + this->list[fd].getNick() + " JOIN " + it2->first + "\r\n", fd);
			this->_channels[it2->first].sendMessage(":" + this->_hostname + " 332 " + it->second.getNick() + " " + it2->first + " :" + this->_channels[it2->first].getTopic() + "\r\n", -1);
			this->_channels[it2->first].sendMessage(":" + this->_hostname + " 353 " + it->second.getNick() + " = " + it2->first + " :" + this->_channels[it2->first].getUsers() + "\r\n", -1);
			this->_channels[it2->first].sendMessage(":" + this->_hostname + " 366 " + it->second.getNick() + " " + it2->first + " :End of /NAMES list\r\n", -1);

		}
	}
}