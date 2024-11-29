cmake_minimum_required(VERSION 3.10)
project(test_project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include       # Project-wide includes
    ${CMAKE_SOURCE_DIR}/include/rfl   # Include for rfl headers
    ${CMAKE_SOURCE_DIR}               # If any files are at the root
)

# Add some options (optional)
set(YYJSON_DISABLE_NON_STANDARD ON CACHE INTERNAL "")

# Add the `yyjson` subdirectory
add_subdirectory(yyjson)

# Add source files
set(SOURCES
    main.cpp
    src/reflectcpp.cpp
    src/reflectcpp_json.cpp
)

# Add executable
add_executable(test ${SOURCES})

# Link yyjson to the `test` target
target_link_libraries(test PRIVATE yyjson)

# If 'rfl' is a separate library, link it
# target_link_libraries(test PRIVATE rfl)  # Uncomment if 'rfl' is a separate library
