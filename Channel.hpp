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
		void addClient(Client A);
		void kickClient(std::string name);
		void kickOperator(int fd);
		void setOperator(int fd);
		void setPassword(std::string pass);
		void setPrivate(bool switcher);
		void setLocked(bool switcher);
		void setTopic(std::string topic);
		void setSizeLimit(int size);
		void setLimited(bool switcher);
		void sendMessage(std::string mssg);
		std::string getPassword();
		int numberOfClients();
		int getSizeLimit();
		int getOwner();

};

/*MODE - Change the channel’s mode:
	· i: Set/remove Invite-only channel // !done
	//!· t: Set/remove the restrictions of the TOPIC command to channel	operators: WTF is this?
	· k: Set/remove the channel key (password)   // !done
	· o: Give/take channel operator privilege   // !done 
											   //  ! no other operator can kick the owner or take his operator privilege
	 l: Set/remove the user limit to channe*/ //    !done

#endif
