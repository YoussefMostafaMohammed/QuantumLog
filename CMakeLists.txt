cmake_minimum_required(VERSION 3.22)
project(QuantumLog)

# Include Conan-generated toolchain
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

set(TOOLCHAIN_PREFIX "/usr/bin/g++")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX})

file(GLOB SOURCES "src/*.cpp")
add_executable(QuantumLog ${SOURCES})
target_include_directories(QuantumLog PRIVATE inc)

# Link Conan dependencies
find_package(spdlog REQUIRED)
target_link_libraries(QuantumLog PRIVATE spdlog::spdlog)

# AddressSanitizer flags
if(DEFINED ENV{IN_DOCKER} AND NOT $ENV{IN_DOCKER} STREQUAL "")
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
