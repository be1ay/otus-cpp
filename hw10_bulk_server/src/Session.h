#pragma once

#include <boost/asio.hpp>
#include <string>
#include "CommandCollector.h"

class Dispatcher;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket socket,
            Dispatcher& dispatcher,
            std::size_t bulkSize);

    void start();

private:
    void doRead();
    void processLine(const std::string& line);
    void finish();

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buffer_;

    Dispatcher& dispatcher_;
    CommandCollector collector_;

    int braceDepth_ = 0;
};
