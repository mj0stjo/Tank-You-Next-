#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

class Server {
private:
    std::string read(tcp::socket& socket);
    void send(tcp::socket& socket, const std::string& message);
public:
    int start();
};

#endif