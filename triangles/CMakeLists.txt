cmake_minimum_required(VERSION 3.5)

project(triangles)

if (NOT CMAKE_CXX_STANDARD AND NOT CMAKE_CXX_COMPILER)
        set(CMAKE_CXX_STANDARD 20)
endif()

option(TRIANGLES "Building main programm without tests" ON)
option(TESTS "Building main programm with tests" OFF)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

message("CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}")
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

if (TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(TESTS)

add_subdirectory(src)

 