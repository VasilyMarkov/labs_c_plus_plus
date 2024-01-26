message("Triangles building...")

add_executable(triangles 
    main.cpp
)

target_include_directories(triangles 
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)


