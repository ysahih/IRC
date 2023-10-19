#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>



class Server {
	private:
		int _socketfd;
		// std::string _name;
		struct sockaddr_in _addr;
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