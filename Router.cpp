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

        // Handle root ("/") route
        if (request.find("GET / ") != std::string::npos) {
            std::string html_content = "<html><body><h1 style='text-align: center;'>Video Server</h1></body></html>";
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " 
                                    + std::to_string(html_content.size()) + "\r\n\r\n" + html_content;
            boost::asio::write(socket, boost::asio::buffer(response), error);
        }
        // Handle test ("/test") route
        else if (request.find("GET /test ") != std::string::npos) {
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 2\r\n\r\n:)";
            boost::asio::write(socket, boost::asio::buffer(response), error);
        }
        // Handle video request ("/video/{id}")
        else if (request.find("GET /video/") != std::string::npos) {
            std::string videoId = request.substr(request.find("/video/") + 7);
            serve_video(std::move(socket), std::stoi(videoId));
        }
        // Return 404 for unknown routes
        else {
            std::string not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
            boost::asio::write(socket, boost::asio::buffer(not_found), error);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception in request handling: " << e.what() << std::endl;
    }
}
