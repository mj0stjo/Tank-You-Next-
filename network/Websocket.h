#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <beast/core.hpp>
#include <beast/websocket.hpp>
#include <beast/http.hpp>
#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = boost::beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Sends a WebSocket message and prints the response
class Websocket {
public:
	int test(std::string const host, std::string const port, std::string const text);

};

#endif // !WEBSOCKET_H