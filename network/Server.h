#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>


using namespace boost::asio;
using ip::tcp;


class connection_handler : public boost::enable_shared_from_this<connection_handler>
{
private:
    int id;
    tcp::socket sock;
    std::shared_ptr<std::string> senMsg;
    std::shared_ptr<std::string> resMsg;
    std::shared_ptr<std::vector<std::string>> senArr;
    std::shared_ptr<std::mutex> readMutex;
    std::shared_ptr<std::mutex> sendMutex;

public:
    connection_handler(int id, std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::shared_ptr<std::vector<std::string>> senArr, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex, const boost::asio::any_io_executor exec);
    // creating the pointer
    static boost::shared_ptr<connection_handler> create(int id, std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::shared_ptr<std::vector<std::string>> senArr, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex, const boost::asio::any_io_executor exec);
    //socket creation
    tcp::socket& socket();
    void start();
    void read();
    void write();
};

class Server {
private:
    std::shared_ptr<tcp::acceptor> acceptor;
    std::shared_ptr<std::string> senMsg;
    std::shared_ptr<std::vector<std::string>> senArr;
    std::shared_ptr<std::string> resMsg;
    std::shared_ptr<std::mutex> readMutex;
    std::shared_ptr<std::mutex> sendMutex;
    int clientCount = 0;

    tcp::acceptor acceptor_;

    void start_accept();
    void handle_accept(boost::shared_ptr<connection_handler> connection, const boost::system::error_code& err);
public:
    Server(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex, boost::asio::io_service& io_service);
};

#endif