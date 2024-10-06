cmake_minimum_required(VERSION 3.10)
project(VideoStreamingServer)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(video_streaming main.cpp Router.cpp VideoHandler.cpp)
target_link_libraries(video_streaming Boost::system)
