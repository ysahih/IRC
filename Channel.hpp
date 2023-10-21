#ifndef CHENNELL_HPP
#define CHENNELL_HPP

#include "Server.hpp"
#include <vector>

//!not implemented
class Channel {
	private:
		std::string _name;
		std::map<std::string, Client> _list;
		int operator_fd;
	public:
		Channel(std::string name);
		bool clientExist(Client A);
		void sendMessage(std::string mssg);
		void addClient(Client A);
		void kickClient(std::string name);
};


#endif
