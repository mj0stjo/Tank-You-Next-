#include "Server.h"

// Inspiration: https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T


Server::Server(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex, boost::asio::io_service& io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234)) {
	this->senMsg = senMsg;
	this->resMsg = resMsg;
	this->readMutex = readMutex;
	this->sendMutex = sendMutex;

	this->senArr = std::make_shared<std::vector<std::string>>();


	/*
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
	*/

	std::cout << "Initialized Server" << std::endl;
	start_accept();

}

void Server::start_accept()
{
	// socket
	boost::shared_ptr<connection_handler> connection = connection_handler::create(clientCount++, senMsg, resMsg, senArr, readMutex, sendMutex, acceptor_.get_executor());

	// asynchronous accept operation and wait for a new connection.
	acceptor_.async_accept(connection->socket(),
		boost::bind(&Server::handle_accept, this, connection,
			boost::asio::placeholders::error));
}

void Server::handle_accept(boost::shared_ptr<connection_handler> connection, const boost::system::error_code& err)
{
	if (!err) {
		std::cout << "New Client connected." << std::endl;
		std::thread([connection]() {
			connection->start();
			}).detach();
	}
	start_accept();
}

/*
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
*/

connection_handler::connection_handler(int id, std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::shared_ptr<std::vector<std::string>> senArr, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex, const boost::asio::any_io_executor exec) : sock(exec) {
	this->id = id;
	this->senMsg = senMsg;
	this->resMsg = resMsg;
	this->readMutex = readMutex;
	this->sendMutex = sendMutex;
	this->senArr = senArr;
}

boost::shared_ptr<connection_handler> connection_handler::create(int id, std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::shared_ptr<std::vector<std::string>> senArr, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex, const boost::asio::any_io_executor exec)
{
	return boost::shared_ptr<connection_handler>(new connection_handler(id, senMsg, resMsg, senArr, readMutex, sendMutex, exec));
}

tcp::socket& connection_handler::socket()
{
	return sock;
}

void connection_handler::start()
{

	while (true) {
		//write operation
		write();
		//read operation
		read();
	}

}


void connection_handler::read() {
	boost::asio::streambuf buf;
	boost::system::error_code error;
	boost::asio::read_until(sock, buf, "\n", error);
	if (error) {
		std::cout << "Server receive failed: " << error.message() << std::endl;
	}
	else {
		std::string data = boost::asio::buffer_cast<const char*>(buf.data());
		std::lock_guard<std::mutex> lg(*readMutex);

		if (senArr->size() <= id)
			senArr->push_back(data);
		else
			senArr->at(id) = data;
			
		*resMsg = "";
		for (int i = 0; i < senArr->size(); i++) {
			*resMsg += (*senArr).at(i) + "X";
		}
		

	}
}

void connection_handler::write() {
	std::string msg;
	{
		std::lock_guard<std::mutex> lg(*sendMutex);
		msg = *senMsg;
		for (int i = 0; i < senArr->size(); i++) {
			if (i == id) continue;
			msg += "X" + (*senArr).at(i);
		}
		msg += "\n";
	}
	boost::system::error_code error;
	boost::asio::write(sock, boost::asio::buffer(msg), error);
	if (error) {
		std::cout << "Server send failed: " << error.message() << std::endl;
	}
	else {
		
	}
}
