cmake_minimum_required(VERSION 3.31)
project(reflect-parser-validator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(YYJSON_DISABLE_NON_STANDARD ON CACHE INTERNAL "")
set(JSON_BuildTests OFF CACHE INTERNAL "")


include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/reflect-cpp/include
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)


add_subdirectory(yyjson)
add_subdirectory(reflect-cpp) 
add_subdirectory(nlohmann_json) 

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

add_executable(app main.cpp tests/validator_test.cpp)
target_link_libraries(app PRIVATE yyjson reflect-cpp nlohmann_json::nlohmann_json GTest::gtest_main)

