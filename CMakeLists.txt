cmake_minimum_required(VERSION 3.22)
project(QuantumLog)

# Include Conan-generated toolchain if it exists
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
endif()

# Set compiler manually if needed
set(TOOLCHAIN_PREFIX "/usr/bin/g++")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX})

# Gather all source files
file(GLOB SOURCES "src/*.cpp")
add_executable(QuantumLog ${SOURCES})

# Include headers
target_include_directories(QuantumLog PRIVATE inc)

# Link dependencies
# spdlog might be provided by Conan or system
find_package(spdlog QUIET)
if(NOT spdlog_FOUND)
    message(STATUS "spdlog not found via find_package, make sure it is installed or provided by Conan")
else()
    target_link_libraries(QuantumLog PRIVATE spdlog::spdlog)
endif()

# AddressSanitizer flags (skip inside Docker)
if(DEFINED ENV{IN_DOCKER} AND NOT "$ENV{IN_DOCKER}" STREQUAL "")
    message(STATUS "Building inside Docker, skipping ASan flags")
else()
    message(STATUS "Adding AddressSanitizer flags")
    target_compile_options(QuantumLog
        PRIVATE -g -fsanitize=address -fno-omit-frame-pointer -O0 -std=c++17 -fdump-lang-class
    )
    target_link_options(QuantumLog
        PRIVATE -fsanitize=address
    )
endif()
