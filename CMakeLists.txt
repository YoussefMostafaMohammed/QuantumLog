cmake_minimum_required(VERSION 3.22)
project(QuantumLog VERSION 1.0 LANGUAGES CXX)

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

# Link dependencies provided by Conan
# Conan generates targets spdlog::spdlog_header_only and fmt::fmt
if(TARGET spdlog::spdlog_header_only AND TARGET fmt::fmt)
    message(STATUS "Linking spdlog and fmt using Conan targets")
    target_link_libraries(QuantumLog PRIVATE spdlog::spdlog_header_only fmt::fmt)
else()
    message(STATUS "spdlog or fmt targets not found via Conan, fallback to find_package")
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

# AddressSanitizer flags (skip inside Docker)
if(DEFINED ENV{IN_DOCKER} AND NOT "$ENV{IN_DOCKER}" STREQUAL "")
    message(STATUS "Building inside Docker, skipping AddressSanitizer flags")
else()
    message(STATUS "Adding AddressSanitizer flags")
    target_compile_options(QuantumLog
        PRIVATE -g -fsanitize=address -fno-omit-frame-pointer -O0 -std=c++17
    )
    target_link_options(QuantumLog
        PRIVATE -fsanitize=address
    )
endif()
