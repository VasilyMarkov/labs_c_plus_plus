cmake_minimum_required(VERSION 3.5)

project(cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ARC_SRC main.cpp arc.h)
set(IDEAL_SRC main.cpp ideal_cache.h)

add_compile_options(${CMAKEc_CXX_FLAGS})

message("CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

add_executable(arc ${ARC_SRC})

target_compile_definitions(arc
    PRIVATE
        ARC_DEF)
        
add_executable(ideal ${IDEAL_SRC})

enable_testing()
add_subdirectory(googletest)
add_executable(test_cache test_main.cpp)
target_link_libraries(test_cache gtest_main gmock_main)
