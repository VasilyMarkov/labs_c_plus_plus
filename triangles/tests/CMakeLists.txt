include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
message("Tests building...")
add_executable(test_triangles test_triangles.cpp)
add_executable(unit_tests unit_tests.cpp)
add_executable(point_tests point_tests.cpp)

target_include_directories(test_triangles
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(unit_tests
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(point_tests
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(test_triangles gtest_main gmock_main)
target_link_libraries(unit_tests gtest_main gmock_main)
target_link_libraries(point_tests gtest_main gmock_main)