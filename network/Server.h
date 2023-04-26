#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

class Server {
private:
    std::shared_ptr<tcp::socket> sock;
    std::shared_ptr<std::string> senMsg;
    std::shared_ptr<std::string> resMsg;
    void read();
    void send();
    void loop();
public:
    Server(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg);
    void start();
};

#endif