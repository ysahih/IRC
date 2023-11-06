#ifndef CHENNELL_HPP
#define CHENNELL_HPP

#include "Server.hpp"
#include <vector>

class Channel {
	private:
		std::string _name;
		std::string _topic;
		std::map<int, Client> _list;
		std::vector<int> _operators;
		//done forget to erase kikced client from _operators
		std::string _password;
		bool _isPrivate; // invite only
		bool _isLimited; 
		bool _isLocked; // password protected
		bool _isTopicRestricted;
		size_t _size_limit;
	public:
		Channel(){};
		Channel(std::string name);
		bool isLimited();
		bool isFull();
		bool isLocked();
		bool isPrivate();
		bool isOperator(int fd);
		bool clientExist(Client A);
		void setOwner();
		void addClient(Client A);
		void kickClient(std::string name);
		void kickOperator(int fd);
		void setOperator(int fd);
		void setPassword(std::string pass);
		void setPrivate(bool switcher);
		void setLocked(bool switcher);
		void setTopic(std::string topic);
		void setSizeLimit(int size);
		std::string getUsers();
		void setLimited(bool switcher);
		void sendMessage(std::string mssg, int fd);
		bool isTopicRestricted();
		void topicRestriction(bool switcher);
		std::string getPassword();
		std::string getTopic();
		int numberOfClients();
		int getSizeLimit();
		int getOwner();

};

#endif
