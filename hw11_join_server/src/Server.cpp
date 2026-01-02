#include "Server.h"
#include "Session.h"
//#include "Database.h"
#include <iostream>
#include "SqliteDatabase.h"

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_context& io,
               unsigned short port)
    : io_(io)
    , acceptor_(io, tcp::endpoint(tcp::v4(), port))
    , db_(std::make_shared<SqliteDatabase>("join.db"))

{
    doAccept();
}

void Server::doAccept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            std::cout<< "New connection"<<std::endl;
            if (!ec) {
                std::make_shared<Session>(
                    std::move(socket), db_
                )->start();
            }
            doAccept();
        }
    );
}
