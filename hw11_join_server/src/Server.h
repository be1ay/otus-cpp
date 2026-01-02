#pragma once

#include <boost/asio.hpp>
#include "IDatabase.h"


class Server {
public:
    Server(boost::asio::io_context& io, unsigned short port);

private:
    void doAccept();
    boost::asio::io_context& io_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<IDatabase> db_;
};
