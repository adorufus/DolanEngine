cmake_minimum_required(VERSION 3.15)

# Workaround for CMake 4.2.1 bug on macOS
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

project(GameEngine VERSION 1.0.0 LANGUAGES CXX C)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Vendor dependencies
add_subdirectory(Vendor)

# Engine library
add_subdirectory(Engine)

# Sandbox application
add_subdirectory(Sandbox)

# Editor application
add_subdirectory(Editor)

