#pragma once

#include <boost/asio.hpp>

class Dispatcher;

class Server {
public:
    Server(boost::asio::io_context& io,
           unsigned short port,
           std::size_t bulkSize,
           Dispatcher& dispatcher);

private:
    void doAccept();

    boost::asio::io_context& io_;
    boost::asio::ip::tcp::acceptor acceptor_;
    Dispatcher& dispatcher_;
    std::size_t bulkSize_;
};
