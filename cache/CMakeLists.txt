cmake_minimum_required(VERSION 3.5)

project(cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(${CMAKEc_CXX_FLAGS})

message("CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

add_executable(arc arc.cpp)
     
add_executable(ideal ideal.cpp)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_cache test_main.cpp)
target_link_libraries(test_cache gtest_main gmock_main)
