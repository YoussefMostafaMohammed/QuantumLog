cmake_minimum_required(VERSION 3.22)

# ============================================================
# Project Configuration
# ============================================================
project(QuantumLog VERSION 1.0 LANGUAGES CXX)

# ============================================================
# Conan Integration
# ============================================================
# Include Conan-generated toolchain if available
if(EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    message(STATUS "Using Conan toolchain: ${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
endif()

# ============================================================
# Compiler Setup
# ============================================================
# Set compiler manually (optional)
set(TOOLCHAIN_PREFIX "/usr/bin/g++")
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX})

# Enforce modern C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# Source Files
# ============================================================
file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# ============================================================
# Include Directories
# ============================================================
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/inc
)

# ============================================================
# Dependencies
# ============================================================
# Prefer Conan targets, fallback to system find_package
if(TARGET spdlog::spdlog_header_only AND TARGET fmt::fmt)
    message(STATUS "Linking spdlog and fmt (Conan targets found)")
    target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only fmt::fmt)

else()
    message(WARNING "Conan targets not found — attempting system find_package()")

    find_package(spdlog QUIET)
    find_package(fmt QUIET)

    if(TARGET spdlog::spdlog_header_only AND TARGET fmt::fmt)
        target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only fmt::fmt)
    elseif(TARGET spdlog::spdlog AND TARGET fmt::fmt)
        target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog fmt::fmt)
    else()
        message(FATAL_ERROR
            "Failed to locate spdlog or fmt. Please run Conan install first."
        )
    endif()
endif()

# ============================================================
# Sanitizer Configuration
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
message(STATUS "Build Type        : ${CMAKE_BUILD_TYPE}")
message(STATUS "Source Directory  : ${PROJECT_SOURCE_DIR}")
message(STATUS "Binary Directory  : ${PROJECT_BINARY_DIR}")
message(STATUS "=====================================================")
message(STATUS "")
