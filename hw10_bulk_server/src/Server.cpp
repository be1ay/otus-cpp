#include "Server.h"
#include "Session.h"
#include "Dispatcher.h"

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_context& io,
               unsigned short port,
               std::size_t bulkSize,
               Dispatcher& dispatcher)
    : io_(io)
    , acceptor_(io, tcp::endpoint(tcp::v4(), port))
    , dispatcher_(dispatcher)
    , bulkSize_(bulkSize)
{
    doAccept();
}

void Server::doAccept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(
                    std::move(socket),
                    dispatcher_,
                    bulkSize_
                )->start();
            }
            doAccept();
        }
    );
}
