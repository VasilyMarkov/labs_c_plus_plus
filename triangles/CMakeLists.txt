cmake_minimum_required(VERSION 3.5)

project(triangles)
set(CMAKE_CXX_STANDARD 23)

message("CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}")
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

enable_testing()

add_subdirectory(tests)
add_subdirectory(src)

 