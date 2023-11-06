#include "Server.h"

// Inspiration: https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T


Server::Server(std::shared_ptr<std::string> senMsg, std::shared_ptr<std::string> resMsg, std::shared_ptr<std::mutex> readMutex, std::shared_ptr<std::mutex> sendMutex, boost::asio::io_service& io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234)) {
	this->senMsg = senMsg;
	this->resMsg = resMsg;
	this->readMutex = readMutex;
	this->sendMutex = sendMutex;

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


	start_accept();
	
}

void Server::start_accept()
{
	// socket
	boost::shared_ptr<connection_handler> connection = connection_handler::create(acceptor_.get_executor().context());

	// asynchronous accept operation and wait for a new connection.
	acceptor_.async_accept(connection->socket(),
		boost::bind(&Server::handle_accept, this, connection,
			boost::asio::placeholders::error));
}

void Server::handle_accept(boost::shared_ptr<connection_handler> connection, const boost::system::error_code& err)
{
	if (!err) {
		connection->start();
	}
	start_accept();
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


boost::shared_ptr<connection_handler> connection_handler::create(boost::asio::execution_context& exec_cont)
{
	return boost::shared_ptr<connection_handler>(new connection_handler(exec_cont));
}


tcp::socket& connection_handler::socket()
{
	return sock;
}

void connection_handler::start()
{
	sock.async_read_some(
		boost::asio::buffer(data, max_length),
		boost::bind(&connection_handler::read,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	std::string msg;
	{
		std::lock_guard<std::mutex> lg(*sendMutex);
		msg = *senMsg + "\n";
	}
	sock.async_write_some(
		boost::asio::buffer(msg, max_length),
		boost::bind(&connection_handler::write,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void connection_handler::read(const boost::system::error_code& err, size_t bytes_transferred)
{
	if (err) {
		std::cout << "Server receive failed: " << err.message() << std::endl;
	}
	else {
		std::lock_guard<std::mutex> lg(*readMutex);
		*resMsg = data;
		std::cout << "Server received message from Client:" << *resMsg << std::endl;
	}
}

void connection_handler::write(const boost::system::error_code& err, size_t bytes_transferred)
{
	
	if (err) {
		std::cout << "Server send failed: " << err.message() << std::endl;
	}
	else {
		std::cout << "Server sent message to Client!" << std::endl;
	}
}
