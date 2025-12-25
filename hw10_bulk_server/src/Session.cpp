#include "Session.h"
#include "Dispatcher.h"
#include <iostream>
using boost::asio::ip::tcp;

Session::Session(tcp::socket socket,
                 Dispatcher& dispatcher,
                 std::size_t bulkSize)
    : socket_(std::move(socket))
    , dispatcher_(dispatcher)
    , collector_(static_cast<int>(bulkSize), dispatcher_)
{
}

void Session::start() {
    doRead();
}

void Session::doRead() {
    auto self = shared_from_this();

    boost::asio::async_read_until(
        socket_,
        buffer_,
        '\n',
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                std::istream is(&buffer_);
                std::string line;
                while (std::getline(is, line)) {
                    if (!line.empty() && line.back() == '\r') {
                        line.pop_back();
                    }
                    processLine(line);
                }
            }
            else {
                finish();
            }
            doRead();
        }
    );
}

void Session::processLine(const std::string& line) {
    if (line == "{") {
        ++braceDepth_;
        collector_.openBrace();
        return;
    }

    if (line == "}") {
        --braceDepth_;
        collector_.closeBrace();
        return;
    }

    if (!line.empty()) {
        collector_.addCommand(line);
        collector_.tryFlushBySize();
    }
}

void Session::finish() {
    collector_.finish();
}
