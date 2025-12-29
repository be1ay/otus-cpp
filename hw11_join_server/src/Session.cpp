#include "Session.h"
#include <iostream>
#include <deque>
#include "Database.h"
using boost::asio::ip::tcp;

Session::Session(tcp::socket socket, std::shared_ptr<Database> db) 
: socket_(std::move(socket)), db_(db)
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

std::deque<std::string> writeQueue_;

void Session::writeResponse(const std::string& data) {
    bool writing = !writeQueue_.empty();
    writeQueue_.push_back("< " + data);

    if (!writing)
        doWrite();
}

void Session::doWrite() {
    auto self = shared_from_this();

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(writeQueue_.front()),
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                writeQueue_.pop_front();
                if (!writeQueue_.empty())
                    doWrite();
            } else {
                finish();
            }
        }
    );
}

std::vector<std::string> Session::split(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void Session::handleInsert(const std::vector<std::string>& tokens) {
    if (tokens.size() != 4) {
        writeResponse("ERR wrong command format\n");
        return;
    }

    const std::string& table = tokens[1];
    int id = std::stoi(tokens[2]);
    const std::string& name = tokens[3];

    std::string error;
    if (!db_->insert(table, id, name, error)) {
        writeResponse("ERR " + error + "\n");
        return;
    }

    writeResponse("OK\n");
}

void Session::handleTruncate(const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2) {
        writeResponse("ERR wrong command format\n");
        return;
    }
    const std::string& table = tokens[1];
    std::string error;
    if (!db_->truncate(table, error)) {
        writeResponse("ERR " + error + "\n");
        return;
    }
    writeResponse("OK\n");
}

void Session::handleIntersection() {
    auto rows = db_->intersection();

    for (auto& [id, a, b] : rows) {
        writeResponse(
            std::to_string(id) + "," + a + "," + b + "\n"
        );
    }

    writeResponse("OK\n");
}


void Session::handleSymmetricDifference()
{
    auto rows = db_->symmetricDifference();

    for (auto& [id, a, b] : rows) {
        writeResponse(
            std::to_string(id) + "," + a + "," + b + "\n"
        );
    }

    writeResponse("OK\n");
}

void Session::processLine(const std::string& line) {
    auto tokens = split(line);

    if (tokens.empty()) {
        writeResponse("ERR empty command\n");
        return;
    }

    const std::string& cmd = tokens[0];

    if (cmd == "INSERT") {
        handleInsert(tokens);
    }
    else if (cmd == "TRUNCATE") {
        handleTruncate(tokens);
    }
    else if (cmd == "INTERSECTION") {
        handleIntersection();
    }
    else if (cmd == "SYMMETRIC_DIFFERENCE") {
        handleSymmetricDifference();
    }
    else {
        writeResponse("ERR unknown command\n");
    }
}


void Session::finish() {
    std::cout<<"Connection lost"<<std::endl;
}
