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
		// std::string _name;
		int _socketfd;
		short	_port;
		struct sockaddr_in _addr;
		struct pollfd _fds[10];
		std::string _password;
		std::vector<Client> list;
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