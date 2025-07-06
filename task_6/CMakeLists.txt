cmake_minimum_required(VERSION 3.31)
project(task_6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,leak -Wall -Wextra")

add_executable(task_6 main.cpp
        format.hpp)