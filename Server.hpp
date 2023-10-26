#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>


// #include <string.h>

// #include <sys/types.h>
// #include <sys/socket.h>

class Channel;
class Client;

class Server {
	private:
		// std::string _name;
		int _socketfd;
		short	_port;
		struct sockaddr_in _addr;
		struct pollfd _fds[1024];
		std::string _password;
		// std::vector<Client> list;
		std::map<int, Client> list;
		std::map<std::string, Channel> _channels;

		void setAddrInfo();
		void bindPort();
	public:
		Server();
		void setSocket();
		~Server();
		
		int findClient(std::string name);
		bool addClient(struct pollfd _poll);
		void launch();
		void setPort(short _port);
		void sendMessage(int fd, std::string msg);
		void privateMsg(int fd, std::stringstream& iss);
		void setNick(int fd, std::stringstream& iss);
		void kick(int fd, std::stringstream& iss);
		void invite(int fd, std::stringstream& iss);
		void topic(int fd, std::stringstream& iss);
		void setUser(int fd, std::stringstream& iss);
		void joinChannel(int fd, std::stringstream& iss);
		void parse(int fd, std::string line);
		void mode(int fd, std::stringstream& iss);
};

#include "Channel.hpp"
#include "Client.hpp"

#endif