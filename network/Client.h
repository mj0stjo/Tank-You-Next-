#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <mutex>
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
    std::shared_ptr<std::mutex> readMutex;
    std::shared_ptr<std::mutex> sendMutex;
    int sentBullets = 0;
    void read();
    void send();
    void loop();
public:
    Client(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::string ipAddr, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex);
    void start();
};

#endif