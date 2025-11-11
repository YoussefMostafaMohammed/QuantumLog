# Use a lightweight Linux image with build tools
FROM ubuntu:22.04

# Install dependencies + Conan
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-pip \
    && pip3 install conan \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
ENV IN_DOCKER=1

# Copy project files into container
COPY . .

# Create build directory
RUN rm -rf build && mkdir -p build

# Detect default Conan profile
RUN conan profile detect

# Install Conan dependencies into build folder
RUN conan install . -of build --build=missing   

# Configure project using CMake
RUN cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

# Build the project
RUN cmake --build build -- -j$(nproc)

# Default command to run the executable
CMD ["./build/QuantumLog"]