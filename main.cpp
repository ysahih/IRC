#include "Server.hpp"

int main(int ac, char **av)
{
    if (ac != 3){
        std::cout << "Invalid arguments" << std::endl;
        return 1;
    }

    try {
        Server server(av[1], av[2]);
        server.launch();
    }catch (const std::runtime_error& e)  {std::cout << e.what() << std::endl;}
    return 0;

}