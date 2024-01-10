cmake_minimum_required(VERSION 3.5)

project(triangles LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TRIANGLES_SRC main.cpp linal.hpp)

add_compile_options(${CMAKE_CXX_FLAGS_RELEASE})

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message("CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS_RELEASE}")

add_executable(triangles ${TRIANGLES_SRC})

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_triangles test_triangles.cpp)
target_link_libraries(test_triangles gtest_main gmock_main)
