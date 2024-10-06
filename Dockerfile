# Use an official C++ build environment with GCC
FROM gcc:11 as build

# Install CMake and Boost dependencies (Boost 1.74 in Ubuntu 22.04)
RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    libboost1.74-dev \
    libboost-system1.74.0 \
    libboost-all-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /usr/src/app

# Copy the current directory into the container
COPY . .

# Create a build directory and build the project
RUN mkdir -p build && cd build && cmake -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Boost-1.74.0 .. && cmake --build .

# Use a smaller base image for running the server
FROM ubuntu:22.04

# Install necessary runtime dependencies (Boost 1.74 and C++ runtime)
RUN apt-get update && apt-get install -y --no-install-recommends \
    libboost-system1.74.0 \
    libstdc++6 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Copy the built executable from the previous stage
COPY --from=build /usr/src/app/build/video_streaming /usr/local/bin/video_streaming

# Expose port 5000 (for your server)
EXPOSE 5000

# Command to run the video streaming server
CMD ["video_streaming"]
