#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

class Server {
private:
    string read_(tcp::socket& socket);
    void send_(tcp::socket& socket, const string& message);
public:
    int start();
};

#endif