#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>



// #include <string.h>

// #include <sys/types.h>
// #include <sys/socket.h>

#define SIZE 512

class Channel;
class Client;

class Server {
	private:
		std::string _hostname;
		int _socketfd;
		short	_port;
		struct sockaddr_in _addr;
		struct pollfd _fds[SIZE];
		std::string _password;
		std::map<int, Client> list;
		std::map<std::string, Channel> _channels;

		void setAddrInfo();
		void bindPort();
	public:
		Server(std::string port, std::string password);
		void setSocket();
		~Server();
		
		int findClient(std::string name);
		std::string addClient(struct pollfd _poll, std::string line);
		void launch();
		void sendToAll(std::string msg);
		void sendMessage(int fd, std::string msg);
		void privateMsg(int fd, std::stringstream& iss);
		void setNick(int fd, std::stringstream& iss);
		void kick(int fd, std::stringstream& iss);
		void invite(int fd, std::stringstream& iss);
		void topic(int fd, std::stringstream& iss);
		void setUser(int fd, std::stringstream& iss);
		void joinChannel(int fd, std::stringstream& iss);
		void quit(int fd);
		void part(int fd, std::stringstream& iss);
		void parse(int fd, std::string line);
		void mode(int fd, std::stringstream& iss);
		std::string getHostName();
		void bot(int fd, std::stringstream &iss);
		void getDate(int fd);
		void getHelp(int fd);
};

std::map<std::string, std::string> collectChannels(std::string names, std::string passwords);
std::string toLower(std::string str);
#include "Channel.hpp"
#include "Client.hpp"

#endif