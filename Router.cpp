#include "Router.hpp"
#include "VideoHandler.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

void handle_request(tcp::socket socket) {
    try {
        // Read the request
        char buffer[1024];
        boost::system::error_code error;
        size_t length = socket.read_some(boost::asio::buffer(buffer), error);

        if (error == boost::asio::error::eof) return; // Connection closed
        else if (error) throw boost::system::system_error(error); // Other errors

        std::string request(buffer, length);
        std::cout << "Request received: " << request << std::endl;

        // Handle video request
        if (request.find("GET /video/") != std::string::npos) {
            std::string videoId = request.substr(request.find("/video/") + 7);
            serve_video(std::move(socket), std::stoi(videoId));
        } else {
            // Return 404 for other routes
            std::string not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
            boost::asio::write(socket, boost::asio::buffer(not_found), error);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in request handling: " << e.what() << std::endl;
    }
}
