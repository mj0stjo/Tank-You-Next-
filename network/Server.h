#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <mutex>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

class Server {
private:
    std::shared_ptr<tcp::socket> sock;
    std::shared_ptr<tcp::acceptor> acceptor;
    std::shared_ptr<std::string> senMsg;
    std::shared_ptr<std::string> resMsg;
    std::shared_ptr<std::mutex> readMutex;
    std::shared_ptr<std::mutex> sendMutex;
    void read();
    void send();
    void loop();
public:
    Server(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex);
};

#endif