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

/*#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string channels = "a,b,c";
    std::vector<std::string> channelList;

    std::string::size_type start = 0;
    std::string::size_type end = 0;

    while ((end = channels.find(',', start)) != std::string::npos) {
        std::string name = channels.substr(start, end - start);
        start = end + 1;
    }
    std::string name = channels.substr(start);

    // Extract the last channel (after the last comma)
    channelList.push_back(lastChannel);

    // Print the individual channels
    for (const auto& channel : channelList) {
        std::cout << channel << std::endl;
    }

    return 0;
}*/


/*MODE - Change the channel’s mode:
	· i: Set/remove Invite-only channel // !done
	//!· t: Set/remove the restrictions of the TOPIC command to channel	operators: WTF is this?
	· k: Set/remove the channel key (password)   // !done
	· o: Give/take channel operator privilege   // !done 
											   //  ! no other operator can kick the owner or take his operator privilege
	 l: Set/remove the user limit to channe*/ //    !done

#endif
