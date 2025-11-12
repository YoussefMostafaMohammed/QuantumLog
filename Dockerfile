# Use a lightweight Linux image with build tools
FROM ubuntu:22.04

# Install dependencies + Conan + Bazel prerequisites
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-pip \
    curl \
    unzip \
    zip \
    openjdk-11-jdk \
    && pip3 install conan \
    && rm -rf /var/lib/apt/lists/*

# Install Bazel
RUN curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > /usr/share/keyrings/bazel-archive-keyring.gpg \
    && echo "deb [signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" \
       | tee /etc/apt/sources.list.d/bazel.list \
    && apt-get update && apt-get install -y bazel

WORKDIR /app
ENV IN_DOCKER=1

# Copy project files into container
COPY . .

# Create build directory
RUN rm -rf build && mkdir -p build

# Detect default Conan profile
RUN conan profile detect

# *********************** using CMake (commented, optional) ***********************
# Install Conan dependencies into build folder
# RUN conan install . -of build --build=missing   

# Configure project using CMake
# RUN cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

# Build the project using CMake
# RUN cmake --build build -- -j$(nproc)

# *********************** using Bazel *************************
RUN conan install . --output-folder=.conan --build=missing
RUN bazel --bazelrc=.conan/conan_bzl.rc build --config=conan-config //src:main

# Default command to run the executable built by Bazel
CMD ["./bazel-bin/src/main"]
