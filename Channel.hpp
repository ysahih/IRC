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
		std::string _password;
		bool _isPrivate; // invite only
		bool _isLimited; 
		bool _isLocked; // password protected
		int _size_limit;
	public:
		Channel(){};
		Channel(std::string name);
		bool isLimited();
		bool isFull();
		bool isLocked();
		bool isPrivate();
		bool isOperator(int fd);
		bool clientExist(Client A);
		void setPassword(std::string pass);
		void addClient(Client A);
		void kickClient(std::string name);
		void setOperator(int fd);
		void setTopic(std::string topic);
		void sendMessage(std::string mssg);
		void setSizeLimit(int size);
		std::string getPassword();
		int numberOfClients();
		int getSizeLimit();
};

/*MODE - Change the channel’s mode:
	· i: Set/remove Invite-only channel // !turn on/off the _isPrivate;
	//!· t: Set/remove the restrictions of the TOPIC command to channel << wtf!
	operators
	· k: Set/remove the channel key (password)   // !turn on/off the _isLocked; and set the _password
	· o: Give/take channel operator privilege   // ! the owner would be the first of operator, 
											   //  ! no other operator can kick the owner or take his operator privilege
	 l: Set/remove the user limit to channe*/ //    !turn on/off the _isLimit;

#endif
