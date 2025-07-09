cmake_minimum_required(VERSION 3.31)
project(task_5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,leak -Wall -Wextra")

add_executable(task_5 main.cpp
        serializer.hpp)

enable_testing()

add_executable(serializer_tests 
        tests/serializer_tests.cpp 
        serializer.hpp)

target_link_libraries(serializer_tests gtest gtest_main pthread)