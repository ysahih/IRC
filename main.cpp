#include "Server.hpp"

int main()
{
    Server server;
    server.setPort(8080);
    try {
        server.launch();
    }catch (const char* s)  {std::cout << s << std::endl; }

}