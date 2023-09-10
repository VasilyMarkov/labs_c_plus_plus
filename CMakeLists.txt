cmake_minimum_required(VERSION 3.5)

project(cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ARC_SRC main.cpp arc.h)

set(IDEAL_SRC main.cpp ideal_cache.h)

add_executable(arc ${ARC_SRC})
target_compile_definitions(arc
    PRIVATE
        ARC_DEF)
        
add_executable(ideal ${IDEAL_SRC})
