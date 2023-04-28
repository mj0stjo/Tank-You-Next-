#include "Client.h"

Client::Client(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::string ipAddr) {
	this->senMsg = senMsg;
	this->resMsg = resMsg;
	this->ip = ipAddr;
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
		*resMsg = data;
		std::cout << "Client received message from Server:" << *resMsg << std::endl;
	}
}

void Client::send() {
	std::string msg = *senMsg + "\n";
	boost::system::error_code error;
	boost::asio::write(*sock, boost::asio::buffer(msg), error);
	if (error) {
		cout << "Clint send failed: " << error.message() << endl;
	}
	else {

		std::cout << "Client sent message to Server!" << std::endl;
	}
}

void Client::start() {
	boost::asio::io_service io_service;
	//socket creation
	sock = std::make_shared<tcp::socket>(io_service);
	//connection
	 sock->connect(tcp::endpoint(boost::asio::ip::address::from_string(ip), 1234));
	// request/message from client
	loop();
}

void Client::loop() {
	int i{ 0 };
	while (true) {
		//read operation
		read();
		//write operation
		send();
	}
}

//int Client::start() {
//    boost::asio::io_service io_service;
//    //socket creation
//    tcp::socket socket(io_service);
//    //connection
//    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(ip), 1234));
//    // request/message from client
//    const string msg = "Hello from Client!\n";
//    boost::system::error_code error;
//    boost::asio::write(socket, boost::asio::buffer(msg), error);
//    if (!error) {
//        cout << "Client sent hello message!" << endl;
//    }
//    else {
//        cout << "send failed: " << error.message() << endl;
//    }
//    // getting response from server
//    boost::asio::streambuf receive_buffer;
//    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
//    if (error && error != boost::asio::error::eof) {
//        cout << "receive failed: " << error.message() << endl;
//    }
//    else {
//        const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
//        cout << data << endl;
//    }
//    return 0;
//}