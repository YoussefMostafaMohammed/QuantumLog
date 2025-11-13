Got it! Here's a fully rewritten README that **matches your single Dockerfile setup** and clearly explains all build scenarios with Docker build args:

---

# Phase One: Core Synchronous Logging Foundation

## Overview

This project implements the foundational skeleton for a C++ logging system. It provides a minimalist prototype, allowing logs to be routed to multiple sinks while adhering to best C++ practices. The architecture is designed to be extendable, containerized, and reproducible across different build environments.

---

## Objectives

1. Define the structure of a log message.
2. Implement a `LogManager` to manage and route log messages.
3. Create concrete log sinks (`ConsoleSinkImpl`, `FileSinkImpl`) that output logs to different targets.
4. Support multiple build systems (CMake, Bazel) and dependency management (Conan).
5. Containerize the project using Docker.

---

## C++ Concepts Covered

* `virtual`, `default`, `friend`, `public`, `protected`, `private` keywords
* Classes, interfaces, inheritance, and polymorphism
* Virtual tables (virtual functions and destructors)
* Operator overloading (`<<`)
* Namespaces and name mangling

---

## Design Patterns

* **Strategy Design Pattern** – decouples log routing logic from log sinks.

---

## Deliverables

### Core Components

* **`ILogSink`** – Pure virtual interface with `write()` method and defaulted virtual destructor.
* **`LogMessage`** – Contains fields: app name, context, timestamp, severity, and text.
* **`LogManager`** – Manages log messages and routes them to all sinks.
* **`ConsoleSinkImpl`** & **`FileSinkImpl`** – Implement `ILogSink` to output to console and file.

### Bonus

* Overload the `<<` operator for `LogMessage` for fully formatted string output (`std::ostream`).

---

## Project Structure

```
QuantumLog/
├── CMakeLists.txt
├── conanfile.py
├── Dockerfile
├── inc/
│   ├── BUILD.bazel
│   ├── *.hpp
├── MODULE.bazel
├── MODULE.bazel.lock
├── notes/
├── src/
│   ├── BUILD.bazel
│   ├── *.cpp
└── README.md
```

---

## Features

* Fully implemented core synchronous logging system
* Dockerized for reproducibility
* Conan for dependency management (spdlog, fmt)
* Supports both Bazel and CMake builds
* Ready for Continuous Integration (CI/CD)

---

## Build Instructions

first install these System Dependencies

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \       # gcc, g++, make
    cmake \                 # CMake build system
    git \                   # git
    python3-pip \           # for pip, Conan (if needed)
    curl \                  # download scripts
    unzip \                 # unzip archives
    zip \                   # zip files
    libspdlog-dev \         # spdlog headers + libs
    libfmt-dev \            # fmt headers + libs
    openjdk-11-jdk \        # required by Bazel
    pkg-config \            # CMake helper to find libraries
    ninja-build \           # optional, faster CMake builds
```

### 1️ CMake Alone

```bash
rm -rf build && mkdir -p build && cd build
cmake ..
cmake --build . -- -j$(nproc)
./QuantumLog
```

---

### 2️ CMake with Conan

```bash
rm -rf build && mkdir build
USE_BAZEL=false conan install . --output-folder=.conan --build=missing
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=.conan/conan_toolchain.cmake
cmake --build build -- -j$(nproc) && ./build/QuantumLog
```

---
 
### 3️ Bazel with conan

> just run 
```bash
conan install . --output-folder=.conan --build=missing -g BazelDeps -g BazelToolchain;
bazel build //src:main
bazel run //src:main
```

    `befor running the commands make sure that src/BUILD.bazel and MODULE.bazel doesn't changed`

### 3️ Bazel Alone without conan


> just run

```bash
sed -i '/load_conan_dependencies/d' MODULE.bazel
sed -i '/use_repo/d' MODULE.bazel
sed -i '/fmt/d' src/BUILD.bazel
sed -i '/spdlog/d' src/BUILD.bazel
bazel build //src:main
bazel run //src:main
```

 `or do it manually `

> from module .bazel remove these lines

```MODULE.bazel
load_conan_dependencies = use_extension("//.conan:conan_deps_module_extension.bzl", "conan_extension")
use_repo(load_conan_dependencies, "fmt", "spdlog")
```

> then the new MODULE.bazel will be 

```MODULE.bazel
module(name = "quantum_log", version = "1.0.0")
bazel_dep(name = "rules_cc", version = "0.2.14")
```

> and from src/BUILD.bazel remove 

```src/BUILD.bazel
"@fmt//:fmt",
"@spdlog//:spdlog",
```

> the new src/BUILD.bazel will be 

```src/BUILD.bazel
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary")

cc_library(
    name = "log_lib",
    srcs = ["ConsoleSinkImpl.cpp", "FileSinkImpl.cpp", "LogManager.cpp", "LogMessage.cpp"],
    hdrs = ["//inc:log_headers"],
    includes = ["../inc"],
    deps = [
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [":log_lib"],
)
```

> then build and run

```bash
bazel build //src:main
bazel run //src:main
```

---

## Docker Builds (Single Dockerfile)

All Docker-based builds use **the same Dockerfile**. You can switch between CMake or Bazel and optionally include Conan by passing build arguments: `BUILD_SYSTEM` and `USE_CONAN`.

### 4️ Docker with CMake Alone

    ```bash
    docker build --build-arg BUILD_SYSTEM=cmake --build-arg USE_CONAN=false -t quantumlog-docker-cmake .
    docker run --rm quantumlog-docker-cmake
```

### 5️ Docker with CMake + Conan

```bash
docker build --build-arg BUILD_SYSTEM=cmake --build-arg USE_CONAN=true -t quantumlog-docker-conan-cmake .
docker run --rm quantumlog-docker-conan-cmake
```

### 6️⃣ Docker with Bazel Alone

```bash
docker build --build-arg BUILD_SYSTEM=bazel --build-arg USE_CONAN=false -t quantumlog-docker-bazel .
docker run --rm quantumlog-docker-bazel
```

### 7️ Docker with Bazel + Conan

```bash
docker build --build-arg BUILD_SYSTEM=bazel --build-arg USE_CONAN=true -t quantumlog-docker-conan-bazel .
docker run --rm quantumlog-docker-conan-bazel
```

---

## Summary

* You can build **natively** with CMake or Bazel.
* You can optionally include **Conan for dependencies**.
* You can optionally use **Docker for reproducible environments**.
* **Single Dockerfile** handles all scenarios via build arguments (`BUILD_SYSTEM` and `USE_CONAN`).
