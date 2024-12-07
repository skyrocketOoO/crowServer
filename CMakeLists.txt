cmake_minimum_required(VERSION 3.31)
project(crowServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(YYJSON_DISABLE_NON_STANDARD ON CACHE INTERNAL "")

# Include Crow and Asio
include_directories(
  asio/include 
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/reflect-cpp/include
)

add_subdirectory(yyjson)
add_subdirectory(reflect-cpp) 

add_library(reflect-cpp STATIC
    reflect-cpp/src/reflectcpp.cpp
    reflect-cpp/src/reflectcpp_json.cpp
)

set(
  SOURCES
  main.cpp
  src/reflectcpp.cpp
  src/reflectcpp_json.cpp
)

add_executable(app main.cpp include/reflect-utils/validator/rule.cpp)
target_link_libraries(app PRIVATE yyjson reflect-cpp)