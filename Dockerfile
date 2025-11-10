# Use a lightweight Linux image with build tools
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory inside container
WORKDIR /app

ENV IN_DOCKER=1

# Copy project files into container
COPY . .

# Create build directory and remove the previous build

RUN rm -rf build && mkdir -p build

RUN cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build the project
RUN cd build && make -j$(nproc)

# Default command to run the compiled executable
CMD ["./build/QuantumLog"]
