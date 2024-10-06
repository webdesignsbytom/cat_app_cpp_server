#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <boost/asio.hpp>

void handle_request(boost::asio::ip::tcp::socket socket);

#endif
