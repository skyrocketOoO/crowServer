cmake_minimum_required(VERSION 3.20)

# Project name
project(crowServer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(. asio/include)

# Add the executable target
add_executable(app 
    main.cpp 
    validator/validator.cpp
)

add_compile_options(-std=c++20)


# Link additional libraries if needed (for Crow dependencies)
# Assuming you need OpenSSL for Crow
find_package(OpenSSL REQUIRED)
target_link_libraries(app PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Ensure Asio or other libraries are properly included
target_include_directories(app PRIVATE asio/include)

# Optionally set warnings and debug options for development
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(app PRIVATE -Wall -Wextra -pedantic)
endif()
