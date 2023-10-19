#include "Server.hpp"

int main()
{
    Server server;
    server.setPort(8080);
    server.launch();
}