cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(QuantumLog VERSION 1.0 LANGUAGES CXX)

# -------------------------------
# Compiler and C++ standard
# -------------------------------
set(CMAKE_CXX_COMPILER /opt/gcc-15.2/bin/g++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable GCC experimental modules
add_compile_options(-fmodules-ts)

# -------------------------------
# Project sources
# -------------------------------
file(GLOB SOURCES "src/*.cpp")
add_executable(QuantumLog ${SOURCES})

target_include_directories(QuantumLog PRIVATE inc)

# -------------------------------
# Conan libraries
# -------------------------------
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
endif()

if(TARGET spdlog::spdlog_header_only AND TARGET fmt::fmt)
    target_link_libraries(QuantumLog PRIVATE spdlog::spdlog_header_only fmt::fmt)
else()
    find_package(spdlog QUIET)
    find_package(fmt QUIET)
    if(TARGET spdlog::spdlog_header_only AND TARGET fmt::fmt)
        target_link_libraries(QuantumLog PRIVATE spdlog::spdlog_header_only fmt::fmt)
    elseif(TARGET spdlog::spdlog AND TARGET fmt::fmt)
        target_link_libraries(QuantumLog PRIVATE spdlog::spdlog fmt::fmt)
    else()
        message(FATAL_ERROR "Could not find spdlog or fmt libraries. Ensure Conan install was successful.")
    endif()
endif()

# -------------------------------
# AddressSanitizer (skip Docker)
# -------------------------------
if(NOT DEFINED ENV{IN_DOCKER} OR "$ENV{IN_DOCKER}" STREQUAL "")
    target_compile_options(QuantumLog PRIVATE -g -fsanitize=address -fno-omit-frame-pointer -O0)
    target_link_options(QuantumLog PRIVATE -fsanitize=address)
endif()
