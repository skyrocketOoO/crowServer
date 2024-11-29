cmake_minimum_required(VERSION 3.10)
project(test_project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    main.cpp
    src/reflectcpp.cpp
    src/reflectcpp_json.cpp
    # Add other source files as needed
)

# Add executable
add_executable(test ${SOURCES})
