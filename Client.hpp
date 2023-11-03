#ifndef CLIENT_HPP
#define CLIENT_HPP

// #include "Server.hpp"
#include <iostream>
#include <map>
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
#include <arpa/inet.h>



class Client {
	private:
		int _sockfd;
		struct pollfd fd;
		std::string IPAddr;
		struct sockaddr_in _addr;
		std::string _username;
		std::string _nickname;
		std::string _buffer;
		bool _authenticate;
		bool _welcomed;
		bool _registered;

	public:
		Client();
		void authenticate();
		bool isAuthenticate();
		bool isWelcomed();
		bool isRegistered();
		void clearBuffer();
		bool isBuffered();
		std::string joinBuffer(std::string msg);
		void setRegistered(bool registered);
		void initClient(struct pollfd _poll, struct sockaddr_in _addr);
		void setUser(std::string name);
		void setWelcomed(bool welcomed);
		void setNick(std::string name);
		struct sockaddr_in getAddr();
		std::string getNick();
		std::string getIP();
		std::string getUser();
		int getFd();

};


#endif