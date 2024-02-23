#include "Client.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

Client::Client(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::string ipAddr, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex) {
	this->senMsg = senMsg;
	this->resMsg = resMsg;
	this->ip = ipAddr;
	this->readMutex = readMutex;
	this->sendMutex = sendMutex;

	boost::asio::io_service io_service;
	//socket creation
	sock = std::make_shared<tcp::socket>(io_service);

	std::cout << "Initialized Client" << std::endl;

	start();
}

void Client::read() {
	boost::asio::streambuf receive_buffer;
	boost::system::error_code error;
	boost::asio::read_until(*sock, receive_buffer, "\n", error);
	if (error) {
		cout << "Cient receive failed: " << error.message() << endl;
	}
	else {
		const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
		std::lock_guard<std::mutex> lg(*readMutex);
		*resMsg = data;
	}
}

void Client::send() {
	std::string msg;
	{
		std::lock_guard<std::mutex> lg(*sendMutex);
		 msg = *senMsg + "\n";
	}

	std::vector<std::string> posAndBul;

	boost::split(posAndBul, msg, boost::is_any_of("B"), boost::token_compress_on);

	std::string sendMsg;
	sendMsg = posAndBul[0];


	for (int i = 1; i < posAndBul.size(); i++) {
		std::vector<std::string> vals;
		boost::split(vals, posAndBul[i], boost::is_any_of(" "), boost::token_compress_on);
		if (this->sentBullets < vals[6].length()) {
			sendMsg += "B" + posAndBul[i];
			this->sentBullets = vals[6].length();
		}
	}

	std::cout << sendMsg << std::endl;
	std::cout << "~~~~~" << std::endl;
	std::cout << this->sentBullets << std::endl;
	std::cout << "---------------" << std::endl;

	boost::system::error_code error;
	boost::asio::write(*sock, boost::asio::buffer(sendMsg), error);
	if (error) {
		cout << "Clint send failed: " << error.message() << endl;
	}
	else {

	}
}

void Client::start() {
	//connection
	sock->connect(tcp::endpoint(boost::asio::ip::address::from_string(ip), 1234));
	std::cout << "Started Client" << std::endl;
	// request/message from client
	loop();
}

void Client::loop() {
	while (true) {
		//read operation
		read();
		//write operation
		send();
	}
}
