#include "VideoHandler.hpp"
#include <boost/asio.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

using boost::asio::ip::tcp;

void serve_video(tcp::socket socket, int videoId) {
    try {
        std::string videoPath = "/path/to/videos/video" + std::to_string(videoId) + ".mp4";

        if (!std::filesystem::exists(videoPath)) {
            std::string not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
            boost::asio::write(socket, boost::asio::buffer(not_found));
            return;
        }

        std::ifstream videoFile(videoPath, std::ios::binary);
        if (!videoFile.is_open()) {
            std::cerr << "Failed to open video file." << std::endl;
            return;
        }

        // Respond with headers for video content
        std::string header = "HTTP/1.1 200 OK\r\nContent-Type: video/mp4\r\n\r\n";
        boost::asio::write(socket, boost::asio::buffer(header));

        // Send the file in chunks
        char buffer[8192];
        while (!videoFile.eof()) {
            videoFile.read(buffer, sizeof(buffer));
            std::streamsize bytes_read = videoFile.gcount();
            boost::asio::write(socket, boost::asio::buffer(buffer, bytes_read));
        }

        videoFile.close();
    } catch (std::exception& e) {
        std::cerr << "Exception in serving video: " << e.what() << std::endl;
    }
}
