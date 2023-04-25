#include "Server.h"

std::string Server::read(tcp::socket& socket) {
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, "\n");
    std::string data = boost::asio::buffer_cast<const char*>(buf.data());
    return data;
}

void Server::send(tcp::socket& socket, const std::string& message) {
    const std::string msg = message + "\n";
    boost::asio::write(socket, boost::asio::buffer(message));
}

int Server::start() {
    boost::asio::io_service io_service;
    //listen for new connection
    tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234));
    //socket creation 
    tcp::socket socket_(io_service);
    //waiting for connection
    acceptor_.accept(socket_);
    //read operation
    std::string message = read(socket_);
    std::cout << message << std::endl;
    //write operation
    send(socket_, "Hello From Server!");
    std::cout << "Servent sent Hello message to Client!" << std::endl;
    return 0;
}