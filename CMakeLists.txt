cmake_minimum_required(VERSION 3.5)

project(cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEFINE_ARC TRUE)

set(SOURCES main.cpp)

if(DEFINE_ARC)
    add_compile_definitions(DEFINE_ARC)
    add_executable(arc ${SOURCES})
else()
    add_executable(ideal_cache ${SOURCES})
endif()
