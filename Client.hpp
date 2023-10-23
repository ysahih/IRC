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


class Client {
    private:
        int _sockfd;
        struct pollfd fd;
        //
        struct sockaddr_in _addr;
        std::string _username;
        std::string _nickname;
        bool _authenticate;

    public:
        Client();
        void authenticate();
        bool is_authenticate();
        void initClient(struct pollfd _poll);
        void setUser(std::string name);
        void setNick(std::string name);
        std::string getNick();
        std::string getUser();
        int getFd();

};


#endif