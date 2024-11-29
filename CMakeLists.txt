cmake_minimum_required(VERSION 3.10)
project(crowServer)

set(CMAKE_CXX_STANDARD 20)

# Include Crow and Asio
include_directories(. asio/include)

add_executable(app main.cpp)
