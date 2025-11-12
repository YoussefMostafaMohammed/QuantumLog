# Use a lightweight Linux image with build tools
FROM ubuntu:22.04

# Install dependencies + Conan + Bazel prerequisites + system deps for no-Conan
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-pip \
    curl \
    unzip \
    libspdlog-dev \
    libfmt-dev \
    zip \
    openjdk-11-jdk \
    && pip3 install --upgrade pip \
    && pip3 install conan \
    && rm -rf /var/lib/apt/lists/*

# Install Bazel
RUN curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > /usr/share/keyrings/bazel-archive-keyring.gpg \
    && echo "deb [signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" \
       | tee /etc/apt/sources.list.d/bazel.list \
    && apt-get update && apt-get install -y bazel

# Set working directory
WORKDIR /app

# Build args: choose build system and Conan usage
ENV IN_DOCKER=1
ARG BUILD_SYSTEM=cmake          # Options: cmake, bazel
ARG USE_CONAN=true              # Options: true, false
ENV BUILD_SYSTEM=${BUILD_SYSTEM}
ENV USE_CONAN=${USE_CONAN}

# Copy project files into container
COPY . .

# Create build directory
RUN rm -rf build && mkdir -p build

# Detect default Conan profile
RUN conan profile detect

# Install dependencies if USE_CONAN=true and generate build system files
RUN if [ "$USE_CONAN" = "true" ]; then \
        if [ "$BUILD_SYSTEM" = "cmake" ]; then \
            conan install . --output-folder=.conan --build=missing -g CMakeToolchain -g CMakeDeps; \
        elif [ "$BUILD_SYSTEM" = "bazel" ]; then \
            conan install . --output-folder=.conan --build=missing -g BazelDeps -g BazelToolchain; \
        else \
            echo "Unknown BUILD_SYSTEM=$BUILD_SYSTEM"; exit 1; \
        fi \
    else \
        sed -i '/load_conan_dependencies/d' MODULE.bazel; \
        sed -i '/use_repo/d' MODULE.bazel; \
        sed -i '/fmt/d' src/BUILD.bazel; \
        sed -i '/spdlog/d' src/BUILD.bazel; \
        echo "Patched BUILD files for no-Conan mode"; \
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
            bazel build //src:main --check_direct_dependencies=off; \
        fi; \
    else \
        echo "Unknown BUILD_SYSTEM=$BUILD_SYSTEM"; exit 1; \
    fi

# Default command
CMD ["sh", "-c", "if [ \"$BUILD_SYSTEM\" = \"cmake\" ]; then ./build/QuantumLog; else ./bazel-bin/src/main; fi"]