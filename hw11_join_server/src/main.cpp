#include <iostream>
#include <boost/asio.hpp>

#include "Server.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: bulk_server <port> \n";
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));
    boost::asio::io_context io;


    Server server(io, port);

    io.run();

    return 0;
}
