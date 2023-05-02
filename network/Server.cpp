#include "Server.h"

Server::Server(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex) {
	this->senMsg = senMsg;
	this->resMsg = resMsg;
	this->readMutex = readMutex;
	this->sendMutex = sendMutex;

	boost::asio::io_service io_service;
	//listen for new connection
	acceptor = std::make_shared<tcp::acceptor>(io_service, tcp::endpoint(tcp::v4(), 1234));
	//socket creation 
	sock = std::make_shared<tcp::socket>(io_service);

	std::cout << "Initialized Server" << std::endl;

	//waiting for connection
	acceptor->accept(*sock);

	std::cout << "Started server" << std::endl;

	loop();
}

void Server::read() {
	boost::asio::streambuf buf;
	boost::system::error_code error;
	boost::asio::read_until(*sock, buf, "\n", error);
	if (error) {
		std::cout << "Server receive failed: " << error.message() << std::endl;
	}
	else {
		std::string data = boost::asio::buffer_cast<const char*>(buf.data());
		std::lock_guard<std::mutex> lg(*readMutex);
		*resMsg = data;
		std::cout << "Server received message from Client:" << *resMsg << std::endl;
	}
}

void Server::send() {
	std::string msg;
	{
		std::lock_guard<std::mutex> lg(*sendMutex);
		msg = *senMsg + "\n";
	}
	boost::system::error_code error;
	boost::asio::write(*sock, boost::asio::buffer(msg), error);
	if (error) {
		std::cout << "Server send failed: " << error.message() << std::endl;
	}
	else {
		std::cout << "Server sent message to Client!" << std::endl;
	}
}

void Server::loop() {
	while (true) {
		//write operation
		send();
		//read operation
		read();
	}
}