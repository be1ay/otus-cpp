#pragma once

#include <boost/asio.hpp>
#include <string>

class Database;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket socket, std::shared_ptr<Database> db);

    void start();

private:
    void doRead();
    void writeResponse(const std::string& data);
    void doWrite();
    std::vector<std::string> split(const std::string& line);
    void handleInsert(const std::vector<std::string>& tokens);
    void handleTruncate(const std::vector<std::string>& tokens);
    void handleIntersection();
    void handleSymmetricDifference();

    void processLine(const std::string& line);
    void finish();


    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buffer_;
    std::shared_ptr<Database> db_;
};
