cmake_minimum_required(VERSION 3.22)

project(QuantumLog)

set(TOOLCHAIN_PREFIX "/usr/bin/g++")

set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX})

file(GLOB SOURCES "src/*.cpp")

add_executable(QuantumLog ${SOURCES})

target_include_directories(QuantumLog PRIVATE inc)

target_compile_options(QuantumLog
    PRIVATE -g -fsanitize=address -fno-omit-frame-pointer -O0 -std=c++17 -fdump-lang-class
)

target_link_options(QuantumLog
    PRIVATE -fsanitize=address
)
