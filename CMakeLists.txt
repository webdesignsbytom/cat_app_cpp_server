cmake_minimum_required(VERSION 3.10)
project(VideoStreamingServer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Boost configuration: dynamic linking, multithreading enabled, dynamic runtime
set(Boost_USE_STATIC_LIBS        OFF)  
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)

# Find Boost (version 1.74.0 and system component)
find_package(Boost 1.74 REQUIRED COMPONENTS system)

# Find Threads (pthread) library
find_package(Threads REQUIRED)

# Add the executable and link source files
add_executable(video_streaming main.cpp Router.cpp VideoHandler.cpp)

# Link Boost system and pthread libraries
target_link_libraries(video_streaming Boost::system Threads::Threads)
