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
    // private:
    public:
        int _sockfd;
        struct pollfd fd;
        struct sockaddr_in _addr;
        
        // struct sockaddr_in getAddr();
        // void setAddr();
        // Client();
        // ~Client();
};


#endif