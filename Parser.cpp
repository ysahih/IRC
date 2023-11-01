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

void Server::parse(int fd, std::string line){

	std::string str;
	std::stringstream iss(line);
	std::string cmd[11] = {"NICK", "USER", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "PONG", "QUIT", "BOT"};
	iss >> str;
	int index = -1;
    for (int i = 0; i < 11; ++i) {
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
			close(fd); //!
			break;
		case 10:
			this->bot(fd, iss);
			break;
		default:
			this->sendMessage (fd, "421 " + str + " :Unknown command\r\n");
			// throw "Error :invalid cmd\r\n";
	}
}


