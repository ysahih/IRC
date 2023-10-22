#ifndef CHENNELL_HPP
#define CHENNELL_HPP

#include "Server.hpp"
#include <vector>

class Channel {
	private:
		std::string _name;
		std::map<int, Client> _list;
		int operator_fd;
	public:
		Channel(){};
		Channel(std::string name);
		void setOperator(int fd);
		bool clientExist(Client A);
		void sendMessage(std::string mssg);
		void addClient(Client A);
		void kickClient(std::string name);
};




#endif
