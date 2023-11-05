cmake_minimum_required(VERSION 3.5)

project(triangles LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TRIANGLES_SRC main.cpp linal.hpp)

add_compile_options(${CMAKEc_CXX_FLAGS})

message("CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

add_executable(triangles ${TRIANGLES_SRC})
