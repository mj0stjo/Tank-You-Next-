#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

class Client {
private:
    std::shared_ptr<tcp::socket> sock;
    std::shared_ptr<std::string> senMsg;
    std::shared_ptr<std::string> resMsg;
    std::string ip;
    void read();
    void send();
    void loop();
public:
    Client(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::string ipAddr);
    void start();
};

#endif