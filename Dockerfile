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

# Set working directory
WORKDIR /app
ENV IN_DOCKER=1

# Copy project files into container
COPY . .

# Create build directory
RUN rm -rf build && mkdir -p build

# Detect default Conan profile
RUN conan profile detect

# Build args: choose build system and Conan usage
ARG BUILD_SYSTEM=bazel          # Options: cmake, bazel
ARG USE_CONAN=true             # Options: true, false

ENV BUILD_SYSTEM=${BUILD_SYSTEM}
ENV USE_CONAN=${USE_CONAN}

# Install dependencies if USE_CONAN=true
RUN if [ "$USE_CONAN" = "true" ]; then \
        conan install . --output-folder=.conan --build=missing; \
    fi

# Build logic based on build system
RUN if [ "$BUILD_SYSTEM" = "cmake" ]; then \
        if [ "$USE_CONAN" = "true" ]; then \
            cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=.conan/conan_toolchain.cmake; \
        else \
            cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug; \
        fi && \
        cmake --build build -- -j$(nproc); \
    elif [ "$BUILD_SYSTEM" = "bazel" ]; then \
        if [ "$USE_CONAN" = "true" ]; then \
            bazel --bazelrc=.conan/conan_bzl.rc build --config=conan-config //src:main; \
        else \
            bazel build //src:main; \
        fi; \
    else \
        echo "Unknown BUILD_SYSTEM=$BUILD_SYSTEM"; exit 1; \
    fi

# Default command
CMD ["sh", "-c", "if [ \"$BUILD_SYSTEM\" = \"cmake\" ]; then ./build/QuantumLog; else ./bazel-bin/src/main; fi"]
