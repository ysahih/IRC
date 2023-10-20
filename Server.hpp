#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
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


#include "Client.hpp"
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>



class Server {
	private:
		int _socketfd;
		// std::string _name;
		struct sockaddr_in _addr;
		struct pollfd fds[10];
		std::vector<Client> list;
		std::string _password;
		short	_port;
		void setAddrInfo();
		void bindPort();
	public:
		void setSocket();
		Server();
		~Server();
		void launch();
		void setPort(short _port);
		
};


#endif