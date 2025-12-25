#include <iostream>
#include <boost/asio.hpp>

#include "Dispatcher.h"
#include "Server.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: bulk_server <port> <bulk_size>\n";
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));
    std::size_t bulkSize = std::stoul(argv[2]);

    boost::asio::io_context io;

    Dispatcher dispatcher;
    dispatcher.start();

    Server server(io, port, bulkSize, dispatcher);

    io.run();

    dispatcher.stop();
    return 0;
}
