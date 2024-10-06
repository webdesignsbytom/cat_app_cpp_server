# Use an official C++ build environment with gcc and other tools
FROM gcc:11 as build

# Install CMake and Boost dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    libboost-all-dev

# Set the working directory
WORKDIR /usr/src/app

# Copy only CMake configuration and source files first (to cache layers better)
COPY CMakeLists.txt .
COPY main.cpp .
COPY Router.cpp .
COPY Router.hpp .
COPY VideoHandler.cpp .
COPY VideoHandler.hpp .

# Create a build directory
RUN mkdir -p build

# Build the project
RUN cd build && cmake .. && cmake --build .

# Use a smaller base image for the final running container
FROM debian:buster-slim

# Copy the built executable from the build stage
COPY --from=build /usr/src/app/build/video_streaming /usr/local/bin/video_streaming

# Copy video files into the container (adjust path if necessary)
COPY ./videos /videos

# Expose the port that the server will run on (match this to your server)
EXPOSE 5000

# Command to run the video streaming server
CMD ["video_streaming"]
