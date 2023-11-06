#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>


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


class connection_handler : public boost::enable_shared_from_this<connection_handler>
{
private:
    tcp::socket sock;
    std::shared_ptr<std::string> senMsg;
    std::shared_ptr<std::string> resMsg;
    std::shared_ptr<std::mutex> readMutex;
    std::shared_ptr<std::mutex> sendMutex;
    enum { max_length = 1024 };
    char data[max_length];

public:
    connection_handler(boost::asio::io_service& io_service) : sock(io_service) {}
    // creating the pointer
    static boost::shared_ptr<connection_handler> create(boost::asio::io_service& io_service);
    //socket creation
    tcp::socket& socket();
    void start();
    void read(const boost::system::error_code& err, size_t bytes_transferred);
    void write(const boost::system::error_code& err, size_t bytes_transferred);
};

#endif