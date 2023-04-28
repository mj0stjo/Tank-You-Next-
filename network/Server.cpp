#include "Server.h"

Server::Server(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg) {
	this->senMsg = senMsg;
	this->resMsg = resMsg;
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
		*resMsg = data;
	}
}

void Server::send() {
	std::string msg = *senMsg + "\n";
	boost::system::error_code error;
	boost::asio::write(*sock, boost::asio::buffer(msg), error);
	if (error) {
		std::cout << "Server send failed: " << error.message() << std::endl;
	}
}

void Server::start() {
	boost::asio::io_service io_service;
	//listen for new connection
	tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234));
	//socket creation 
	sock = std::make_shared<tcp::socket>(io_service);
	//waiting for connection
	acceptor_.accept(*sock);
	loop();
}

void Server::loop() {
	while (true) {
		//write operation
		send();
		std::cout << "Server sent message to Client!" << std::endl;
		//read operation
		read();
		std::cout << "Server ereived message from Client:" << *resMsg << std::endl;
	}
}