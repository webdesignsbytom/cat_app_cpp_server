#ifndef VIDEO_HANDLER_HPP
#define VIDEO_HANDLER_HPP

#include <boost/asio.hpp>

void serve_video(boost::asio::ip::tcp::socket socket, int videoId);

#endif
