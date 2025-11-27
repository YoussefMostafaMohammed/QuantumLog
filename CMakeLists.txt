cmake_minimum_required(VERSION 3.22)

# ============================================================
# Project Configuration
# ============================================================
project(QuantumLog VERSION 1.0 LANGUAGES CXX)

# ============================================================
# Dependencies - FIND ALL FIRST (create actual targets)
# ============================================================

# Protobuf (system package)
find_package(Protobuf REQUIRED)

# MQTT C library (dependency - you must install this)
# Find via pkg-config or manual search
find_package(PkgConfig QUIET)
pkg_check_modules(PAHO_MQTT_C QUIET paho-mqtt-async)

if(NOT PAHO_MQTT_C_FOUND)
    # Manual fallback for source-installed C library
    find_library(PAHO_MQTT_C_LIB paho-mqtt3a PATHS /usr/local/lib /usr/lib REQUIRED)
    find_path(PAHO_MQTT_C_INCLUDE_DIR MQTTAsync.h PATHS /usr/local/include /usr/include REQUIRED)
    message(STATUS "MQTT C library (manual): ${PAHO_MQTT_C_LIB}")
    message(STATUS "MQTT C headers: ${PAHO_MQTT_C_INCLUDE_DIR}")
else()
    message(STATUS "MQTT C library (pkg-config): ${PAHO_MQTT_C_LIBRARIES}")
endif()

# MQTT C++ library (YOU BUILT THIS FROM SOURCE)
# Since you built from source without cmake config, use manual paths
find_library(PAHO_MQTT_CPP_LIB paho-mqttpp3 PATHS /usr/local/lib REQUIRED)
find_path(PAHO_MQTT_CPP_INCLUDE_DIR mqtt/async_client.h PATHS /usr/local/include REQUIRED)
message(STATUS "MQTT C++ library: ${PAHO_MQTT_CPP_LIB}")
message(STATUS "MQTT C++ headers: ${PAHO_MQTT_CPP_INCLUDE_DIR}")

# JSON (system or /usr/local)
find_package(nlohmann_json REQUIRED)
message(STATUS "nlohmann_json: ${nlohmann_json_VERSION}")

# spdlog and fmt - SYSTEM OR CONAN (preserves your logic)
if(TARGET spdlog::spdlog_header_only)
    set(SPDLOG_TARGET spdlog::spdlog_header_only)
elseif(TARGET spdlog::spdlog)
    set(SPDLOG_TARGET spdlog::spdlog)
else()
    # Fallback: Try pkg-config
    pkg_check_modules(SPDLOG spdlog)
    if(SPDLOG_FOUND)
        add_library(spdlog::spdlog INTERFACE IMPORTED)
        target_include_directories(spdlog::spdlog INTERFACE ${SPDLOG_INCLUDE_DIRS})
        target_link_libraries(spdlog::spdlog INTERFACE ${SPDLOG_LIBRARIES})
        set(SPDLOG_TARGET spdlog::spdlog)
    else()
        message(FATAL_ERROR "spdlog not found. Install via: sudo apt install libspdlog-dev")
    endif()
endif()

if(TARGET fmt::fmt)
    set(FMT_TARGET fmt::fmt)
else()
    pkg_check_modules(FMT fmt)
    if(FMT_FOUND)
        add_library(fmt::fmt INTERFACE IMPORTED)
        target_include_directories(fmt::fmt INTERFACE ${FMT_INCLUDE_DIRS})
        target_link_libraries(fmt::fmt INTERFACE ${FMT_LIBRARIES})
        set(FMT_TARGET fmt::fmt)
    else()
        message(FATAL_ERROR "fmt not found. Install via: sudo apt install libfmt-dev")
    endif()
endif()

# ============================================================
# Compiler Setup
# ============================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# ============================================================
# Source Files + Protobuf Generation
# ============================================================
file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")

get_filename_component(PROTO_FILE_ABSOLUTE "${CMAKE_CURRENT_SOURCE_DIR}/proto/telemetry.proto" ABSOLUTE)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE_ABSOLUTE})
list(APPEND SOURCES ${PROTO_SRCS})

# ============================================================
# Executable Target
# ============================================================
add_executable(${PROJECT_NAME} ${SOURCES})

# ============================================================
# Include Directories - CRITICAL: MQTT headers MUST come first
# ============================================================
# SYSTEM PRIVATE suppresses warnings from 3rd-party headers
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${PAHO_MQTT_CPP_INCLUDE_DIR}  # Modern C++ headers (v1.2+ with callback_functions)
    ${PAHO_MQTT_C_INCLUDE_DIR}    # C library headers (MQTTAsync.h)
    ${CMAKE_CURRENT_BINARY_DIR}   # Generated protobuf headers
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/inc
)

# ============================================================
# Link Libraries - ONE CLEAN COMMAND
# ============================================================
target_link_libraries(${PROJECT_NAME} PRIVATE 
    # Protobuf
    protobuf::libprotobuf
    ${Protobuf_LIBRARIES}
    
    # MQTT (both C++ and its C dependency)
    ${PAHO_MQTT_CPP_LIB}
    ${PAHO_MQTT_C_LIB}
    
    # JSON
    nlohmann_json::nlohmann_json
    
    # Logging
    ${SPDLOG_TARGET}
    ${FMT_TARGET}
    
    # System
    pthread
)

# ============================================================
# Sanitizer Configuration (optional)
# ============================================================
if(DEFINED ENV{IN_DOCKER} AND NOT "$ENV{IN_DOCKER}" STREQUAL "")
    message(STATUS "Building inside Docker — skipping AddressSanitizer flags")
else()
    message(STATUS "Adding AddressSanitizer flags")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -g
            -O0
            -fsanitize=address
            -fno-omit-frame-pointer
    )
    target_link_options(${PROJECT_NAME}
        PRIVATE
            -fsanitize=address
    )
endif()

# ============================================================
# Output Summary
# ============================================================
message(STATUS "")
message(STATUS "================ Build Configuration ================")
message(STATUS "Project Name      : ${PROJECT_NAME}")
message(STATUS "Project Version   : ${PROJECT_VERSION}")
message(STATUS "Compiler          : ${CMAKE_CXX_COMPILER}")
message(STATUS "MQTT C++ Lib      : ${PAHO_MQTT_CPP_LIB}")
message(STATUS "MQTT C Headers    : ${PAHO_MQTT_C_INCLUDE_DIR}")
message(STATUS "JSON              : ${nlohmann_json_VERSION}")
message(STATUS "=====================================================")
message(STATUS "")